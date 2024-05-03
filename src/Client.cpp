#include "Client.h"
#include "ClientConfig.h"
#include "ClientController.h"
#include "events/OrderedPriorityListener.h"
#include "model/UnorderedMapObjectModel.h"
#include "events/handlers/KeyPressHandler.h"
#include "events/handlers/KeyReleaseHandler.h"
#include "events/handlers/ClientCreateCharacterHandler.h"
#include "events/handlers/ClientCreatePlatformHandler.h"
#include "events/handlers/ClientCreateMovingPlatformHandler.h"
#include "events/handlers/ClientReadAssetHandler.h"
#include "events/handlers/ClientUpdateAssetHandler.h"
#include "events/handlers/ClientDeleteAssetHandler.h"

#include <thread>
#include <chrono>

#include <sstream>
#include <cereal/archives/json.hpp>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat;

void Client::handle_key_event(sf::Keyboard::Key key) {
#ifdef DEBUG
    std::cout << "-> Client::handle_key_event(sf::Keyboard::Key)" << std::endl;
#endif
    EnvironmentInterface& env = get_config().get_environment();
    if (sf::Keyboard::isKeyPressed(key) 
            && env.get_held_keys().count(key) == 0) {
        Events::Command cmd;
        cmd.type = Events::KeyPressHandler::get_event_type();
        std::stringstream ss;
        {
            Events::KeyPressHandler::Args args = { key };
            cereal::JSONOutputArchive oar(ss);
            oar(args);
        }
        cmd.args = ss.str();
        cmd.priority = 0;
        Event event = {
            .command=cmd,
            .sender=get_config().get_environment().get_entrypoint_id()
        };
        foreground_listener.push_event(event);

    }
    if (!sf::Keyboard::isKeyPressed(key)
            && env.get_held_keys().count(key) > 0) {
        Events::Command cmd;
        cmd.type = Events::KeyReleaseHandler::get_event_type();
        std::stringstream ss;
        {
            Events::KeyReleaseHandler::Args args = { key };
            cereal::JSONOutputArchive oar(ss);
            oar(args);
        }
        cmd.args = ss.str();
        cmd.priority = 0;
        Event event = {
            .command=cmd,
            .sender=get_config().get_environment().get_entrypoint_id()
        };
        foreground_listener.push_event(event);
    }
#ifdef DEBUG
    std::cout << "<- Client::handle_key_event" << std::endl;
#endif
}

void wait_for_timeline(Timeline& t, time_t target) {
    if (Client::get_instance().get_config().get_timing_config().get_display_timeline().get_paused()
         || t.get_time() >= target) return;

    return wait_for_timeline(t, target);
}

Client::Client() : config(*new ClientConfig()), 
        object_model(*new Model::UnorderedMapObjectModel()), 
        foreground_listener(*new Events::OrderedPriorityListener()),
        background_listener(*new Events::OrderedPriorityListener()),
        ctl(*new ClientController()) {
    // fix framerate limit env
    get_config().get_timing_config().update_framerate_limit(30);  // default 30

    // set foreground event defaults
    foreground_listener.set_handler(Events::KeyPressHandler
        ::get_event_type(), *new Events::KeyPressHandler());
    foreground_listener.set_handler(Events::KeyReleaseHandler
        ::get_event_type(), *new Events::KeyReleaseHandler());

    // set background event defaults
    background_listener.set_handler(Events::ClientCreateCharacterHandler::get_type(),
        *new Events::ClientCreateCharacterHandler());
    background_listener.set_handler(Events::ClientCreatePlatformHandler::get_type(),
        *new Events::ClientCreatePlatformHandler());
    background_listener.set_handler(Events::ClientCreateMovingPlatformHandler
        ::get_type(), *new Events::ClientCreateMovingPlatformHandler());
    background_listener.set_handler(Events::ClientReadAssetHandler::get_type(), *new
        Events::ClientReadAssetHandler());
    background_listener.set_handler(Events::ClientUpdateAssetHandler::get_type(), 
        *new Events::ClientUpdateAssetHandler());
    background_listener.set_handler(Events::ClientDeleteAssetHandler::get_type(), 
        *new Events::ClientDeleteAssetHandler());
}

bool compare(SPlat::Model::Moving* lhs, SPlat::Model::Moving* rhs) {
    return lhs->get_asset_properties().get_collision_priority()
        < rhs->get_asset_properties().get_collision_priority();
}

void Client::start() {
#ifdef DEBUG
    std::cout << "-> Client::start()" << std::endl;
#endif

    ctl.run();

    EnvironmentInterface& environment = get_config().get_environment();
    size_t id;
    do {
        id = environment.get_entrypoint_id();
    } while (id == 0);

    foreground_listener.run();
    background_listener.run();

    window.create(sf::VideoMode(800, 600), "SPlat");

    std::this_thread::sleep_for(std::chrono::milliseconds(250));  // give window time to open
    
    get_config().get_timing_config().get_display_timeline().unpause();  // paused by default
    time_t last_updated = get_config().get_timing_config()
        .get_display_timeline().get_time();

    // prototype update event
    Event prototype = {
        .command = {
            .priority=-1,
            .type=Events::UpdateAssetHandler::get_type()
        },
        .sender=environment.get_entrypoint_id()
    };

    while (window.isOpen()) {
        // poll for closed event
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // check for keyboard inputs
        if (window.hasFocus()) {
            handle_key_event(sf::Keyboard::Key::Left);
            handle_key_event(sf::Keyboard::Key::Right);
            handle_key_event(sf::Keyboard::Key::Up);
            handle_key_event(sf::Keyboard::Key::Escape);
        }

        window.clear(sf::Color::Black);

        std::unordered_set<size_t> ids = object_model.get_ids();
        for (size_t id : ids) {
            // get asset
            Model::Asset& asset = object_model.read_asset(id);

            Model::AssetProperties& asset_properties 
                = asset.get_asset_properties();

            try {
                if (asset_properties.get_owner() 
                        == environment.get_entrypoint_id()) {
                    // attempt routine update
                    Model::Moving& moving 
                        = dynamic_cast<Model::Moving&>(asset);
                    moving.update();
                    moving.get_moving_properties()
                        .set_last_update(last_updated);
                    asset_properties.set_updated_time(last_updated);
                    for (size_t other_id : ids) {
                        if (id == other_id) continue;
                        Model::Asset& other_asset 
                            = object_model.read_asset(other_id);
                        asset.resolve_collision(other_asset);
                    }

                    // send send routine update
                    Event update_event = prototype;
                    Events::UpdateAssetHandler::Args args = {
                        .id=id,
                        .properties=asset_properties
                    };
                    std::stringstream args_ss;
                    {
                        cereal::JSONOutputArchive oar(args_ss);
                        oar(args);
                    }
                    update_event.command.args = args_ss.str();
                    std::stringstream event_ss;
                    {
                        cereal::JSONOutputArchive oar(event_ss);
                        oar(update_event);
                    }
                    Request update_request = {
                        .content_type=Request::ContentType::Event,
                        .body=event_ss.str()
                    };
                    get_controller().push_outgoing_request(update_request);
                }
            } catch (std::bad_cast&) {}

            // draw to screen
            window.draw(asset_properties.get_rectangle_shape());
        }

        window.display();
        wait_for_timeline(get_config().get_timing_config()
            .get_display_timeline(), last_updated + 1);
        last_updated = get_config().get_timing_config().get_display_timeline().get_time();
    }

    get_config().get_environment().set_running(false);
#ifdef DEBUG
    std::cout << "<- Client::start" << std::endl;
#endif
}

ConfigInterface& Client::get_config() {
    const std::lock_guard<std::mutex> lock(m);
    return config;
}

Events::ListenerInterface& Client::get_foreground_listener() {
    const std::lock_guard<std::mutex> lock(m);
    return foreground_listener;
}

Client& Client::get_instance() {
    if (instance == nullptr)
        instance = new Client();
    try {
        return *dynamic_cast<Client*>(instance);
    } catch (std::bad_cast&) {
        throw std::invalid_argument("Entrypoint is not of type Client");
    }
}

Model::ObjectModelInterface& Client::get_object_model() {
    const std::lock_guard<std::mutex> lock(m);
    return object_model;
}

Events::ListenerInterface& Client::get_background_listener() {
    const std::lock_guard<std::mutex> lock(m);
    return background_listener;
}

ControllerInterface& Client::get_controller() {
    const std::lock_guard<std::mutex> lock(m);
    return ctl;
}
