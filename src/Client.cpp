#include "Client.h"
#include "Controller.h"

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

void Client::start() {
#ifdef DEBUG
    std::cout << "-> Client::start()" << std::endl;
#endif
    std::pair<bool, std::mutex>& runtime = *new std::pair<bool, std::mutex>();
    runtime.first = true;

    std::thread t(&Controller::run, &ctl, std::ref(runtime));

    window.create(sf::VideoMode(800, 600), "SPlat");
    window.setFramerateLimit(60);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(250));  // give controller time to get started

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

        // generate tick events
        Events::TickEvent tick_event;
        tick_event.raise();

        // draw all assets
        window.clear(sf::Color::Black);

        std::unordered_set<size_t> asset_ids = Model::GameObjectModel::
            get_instance().getIds();
        for (size_t id : asset_ids) {
            Model::Asset& asset = Model::GameObjectModel::get_instance()
                .read_asset(id);
            window.draw(asset);
        }

        window.display();
    }

    runtime.second.lock();
    runtime.first = false;
    runtime.second.unlock();

    t.join();
#ifdef DEBUG
    std::cout << "<- Client::start" << std::endl;
#endif
}