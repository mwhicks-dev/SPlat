#include "Client.h"
#include "ClientConfig.h"
#include "events/OrderedPriorityListener.h"
#include "model/UnorderedMapObjectModel.h"
#include "events/KeyPressCommandHandler.h"
#include "events/KeyReleaseCommandHandler.h"

#include <thread>
#include <chrono>

#include <sstream>
#include <cereal/archives/json.hpp>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat;

void Client::handle_key_event(sf::Keyboard::Key key) {
    EnvironmentInterface& env = get_config().get_environment();
    if (sf::Keyboard::isKeyPressed(key) 
            && env.get_held_keys().count(key) == 0) {
        Events::Command cmd;
        cmd.type = Events::KeyPressCommandHandler::get_event_type();
        std::stringstream ss;
        {
            Events::KeyPressCommandHandler::Args args = { key };
            cereal::JSONOutputArchive oar(ss);
            oar(args);
        }
        cmd.args = ss.str();
        cmd.priority = 0;
        foreground_listener.push_command(cmd);

    }
    if (!sf::Keyboard::isKeyPressed(key)
            && env.get_held_keys().count(key) > 0) {
        Events::Command cmd;
        cmd.type = Events::KeyReleaseCommandHandler::get_event_type();
        std::stringstream ss;
        {
            Events::KeyReleaseCommandHandler::Args args = { key };
            cereal::JSONOutputArchive oar(ss);
            oar(args);
        }
        cmd.args = ss.str();
        cmd.priority = 0;
        foreground_listener.push_command(cmd);
    }
}

void wait_for_timeline(Timeline& t, time_t target) {
    if (Client::get_instance().get_config().get_timing_config().get_display_timeline().get_paused()
         || t.get_time() >= target) return;

    return wait_for_timeline(t, target);
}

Client::Client() : config(*new ClientConfig()), 
        object_model(*new Model::UnorderedMapObjectModel()), 
        foreground_listener(*new Events::OrderedPriorityListener()) {
    // fix framerate limit env
    get_config().get_timing_config().update_framerate_limit(30);  // default 30

    // set default handlers for key press/release events
    foreground_listener.set_handler(Events::KeyPressCommandHandler
        ::get_event_type(), *new Events::KeyPressCommandHandler());
    foreground_listener.set_handler(Events::KeyReleaseCommandHandler
        ::get_event_type(), *new Events::KeyReleaseCommandHandler());
}

bool compare(SPlat::Model::Moving* lhs, SPlat::Model::Moving* rhs) {
    return lhs->get_asset_properties().get_collision_priority()
        < rhs->get_asset_properties().get_collision_priority();
}

void Client::start() {
#ifdef DEBUG
    std::cout << "-> Client::start()" << std::endl;
#endif

    foreground_listener.run();

    window.create(sf::VideoMode(800, 600), "SPlat");

    std::this_thread::sleep_for(std::chrono::milliseconds(250));  // give window time to open
    
    get_config().get_timing_config().get_display_timeline().unpause();  // paused by default
    time_t last_updated = get_config().get_timing_config()
        .get_display_timeline().get_time();
    while (window.isOpen()) {
        // poll for closed event
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // check for keyboard inputs
        handle_key_event(sf::Keyboard::Key::Left);
        handle_key_event(sf::Keyboard::Key::Right);
        handle_key_event(sf::Keyboard::Key::Up);
        handle_key_event(sf::Keyboard::Key::Escape);

        // generate tick events (if unpaused)
        std::unordered_set<size_t> ids = get_object_model().get_ids();
        std::vector<SPlat::Model::Moving*> moving;
        for (size_t id : ids) {
            try {
                SPlat::Model::Moving& asset = dynamic_cast<SPlat::Model::Moving&>(
                    get_object_model().read_asset(id));
                moving.push_back(&asset);
            } catch (std::bad_cast&) {}
        }

        // draw all assets
        window.clear(sf::Color::Black);

        std::unordered_set<size_t> asset_ids = get_object_model().get_ids();
        for (size_t id : asset_ids) {
            Model::Asset& asset = get_object_model()
                .read_asset(id);
            window.draw(asset.get_asset_properties().get_rectangle_shape());
        }
        time_t curr = Client::get_instance().get_config().get_timing_config()
        .get_display_timeline().get_time();
        for (SPlat::Model::Moving* asset_ptr : moving) {
            try {
                // get and update
                SPlat::Model::Moving& asset = *asset_ptr;
                asset.update();
                asset.get_moving_properties().set_last_update(curr);

                size_t id = asset.get_asset_properties().get_id();
                for (size_t other : ids) {
                    if (id == other) continue;

                    SPlat::Model::Asset& other_asset = get_object_model().read_asset(other);
                    asset.resolve_collision(other_asset);
                }
            } catch (std::exception& e) {
    #ifdef DEBUG
                std::cout << e.what() << std::endl;
    #endif
            }
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
    m.lock();
    ConfigInterface& local = config;
    m.unlock();

    return local;
}

Events::ListenerInterface& Client::get_foreground_listener() {
    m.lock();
    Events::ListenerInterface& local = foreground_listener;
    m.unlock();

    return local;
}

Client& Client::get_instance() {
    if (instance == nullptr)
        instance = new Client();
    try {
        return *dynamic_cast<Client*>(instance);
    } catch (std::bad_cast&) {
        throw std::invalid_argument("Entrypoint is of type Client");
    }
}

Model::ObjectModelInterface& Client::get_object_model() {
    m.lock();
    Model::ObjectModelInterface& local = object_model;
    m.unlock();

    return local;
}