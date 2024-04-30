#include "Client.h"
#include "ClientConfig.h"
#include "events/OrderedPriorityListener.h"
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
        {
            Events::KeyPressCommandHandler::Args args = { key };
            std::stringstream ss;
            cereal::JSONOutputArchive oar(ss);
            oar(args);
            cmd.args = ss.str();
        }
        cmd.priority = 0;
        foreground_listener.push_command(cmd);

    }
    if (!sf::Keyboard::isKeyPressed(key)
            && env.get_held_keys().count(key) > 0) {
        Events::Command cmd;
        cmd.type = Events::KeyReleaseCommandHandler::get_event_type();
        {
            Events::KeyReleaseCommandHandler::Args args = { key };
            std::stringstream ss;
            cereal::JSONOutputArchive oar(ss);
            oar(args);
            cmd.args = ss.str();
        }
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
        foreground_listener(*new Events::OrderedPriorityListener()) {
    // fix framerate limit env
    update_framerate_limit(get_config().get_environment().get_framerate_limit());

    // set default handlers for key press/release events
    foreground_listener.set_handler(Events::KeyPressCommandHandler
        ::get_event_type(), *new Events::KeyPressCommandHandler());
    foreground_listener.set_handler(Events::KeyPressCommandHandler
        ::get_event_type(), *new Events::KeyPressCommandHandler());
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
        std::unordered_set<size_t> ids = Model::GameObjectModel::get_instance().get_ids();
        std::vector<SPlat::Model::Moving*> moving;
        for (size_t id : ids) {
            try {
                SPlat::Model::Moving& asset = dynamic_cast<SPlat::Model::Moving&>(
                    Model::GameObjectModel::get_instance().read_asset(id));
                moving.push_back(&asset);
            } catch (std::bad_cast&) {}
        }

        // draw all assets
        window.clear(sf::Color::Black);

        std::unordered_set<size_t> asset_ids = Model::GameObjectModel::
            get_instance().get_ids();
        for (size_t id : asset_ids) {
            Model::Asset& asset = Model::GameObjectModel::get_instance()
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

                    SPlat::Model::Asset& other_asset = Model::GameObjectModel::get_instance().read_asset(other);
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

void Client::update_framerate_limit(long framerate_limit) {
#ifdef DEBUG
    std::cout << "-> Client::set_framerate_limit(" << framerate_limit << ")" << std::endl;
#endif
    get_config().get_environment().set_framerate_limit(framerate_limit);
    get_config().get_timing_config().get_display_timeline().set_tic(
        get_config().get_timing_config().get_anchor_steps_per_second() / framerate_limit
    );
#ifdef DEBUG
    std::cout << "<- Client::set_framerate_limit" << std::endl;
#endif
}

ConfigInterface& Client::get_config() {
    m.lock();
    ConfigInterface& local = config;
    m.unlock();

    return local;
}

Client& Client::get_instance() {
    static Client client;
    return client;
}