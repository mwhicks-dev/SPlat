#include "Client.h"
#include "Config.h"

#include "events/KeyEvents.h"
#include "events/Listener.h"
#include "events/TickEvent.h"

#include <thread>
#include <chrono>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat;

void Client::handle_key_event(sf::Keyboard::Key key) {
    if (sf::Keyboard::isKeyPressed(key) && !Events::KeyEvent::is_key_pressed(key)) {
        Events::KeyPressEvent press(key);
        press.raise();
    }
    else if (!sf::Keyboard::isKeyPressed(key) && Events::KeyEvent::is_key_pressed(key)) {
        Events::KeyReleaseEvent release(key);
        release.raise();
    }
}

void wait_for_timeline(Timeline& t, time_t target) {
    if (Config::get_instance().get_timing_config().get_display_timeline().get_paused()
         || t.get_time() >= target) return;

    return wait_for_timeline(t, target);
}

Client::Client() {
    update_framerate_limit(Config::get_instance().get_environment().get_framerate_limit());
}

void Client::start() {
#ifdef DEBUG
    std::cout << "-> Client::start()" << std::endl;
#endif

    std::thread t(&Controller::run, &ctl);

    window.create(sf::VideoMode(800, 600), "SPlat");

    std::this_thread::sleep_for(std::chrono::milliseconds(250));  // give window time to open
    
    Config::get_instance().get_timing_config().get_display_timeline().unpause();  // paused by default
    time_t last_updated = Config::get_instance().get_timing_config()
        .get_display_timeline().get_time();
    while (window.isOpen()) {
        // dispatch foreground events
        Events::ForegroundListener::get_instance().run();

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
        Events::TickEvent tick_event;
        tick_event.raise();

        // draw all assets
        window.clear(sf::Color::Black);

        std::unordered_set<size_t> asset_ids = Model::GameObjectModel::
            get_instance().getIds();
        for (size_t id : asset_ids) {
            Model::Asset& asset = Model::GameObjectModel::get_instance()
                .read_asset(id);
            Model::AssetProperties& properties = asset.get_asset_properties();
            sf::RectangleShape rect = properties.get_rectangle_shape();
            window.draw(rect);
        }

        window.display();
        wait_for_timeline(Config::get_instance().get_timing_config()
            .get_display_timeline(), last_updated + 1);
        last_updated = Config::get_instance().get_timing_config().get_display_timeline().get_time();
    }

    Config::get_instance().get_environment().set_running(false);

    t.join();
#ifdef DEBUG
    std::cout << "<- Client::start" << std::endl;
#endif
}

void Client::update_framerate_limit(long framerate_limit) {
#ifdef DEBUG
    std::cout << "-> Client::set_framerate_limit(" << framerate_limit << ")" << std::endl;
#endif
    Config::get_instance().get_environment().set_framerate_limit(framerate_limit);
    Config::get_instance().get_timing_config().get_display_timeline().set_tic(
        Config::get_instance().get_timing_config().get_anchor_steps_per_second() / framerate_limit
    );
#ifdef DEBUG
    std::cout << "<- Client::set_framerate_limit" << std::endl;
#endif
}