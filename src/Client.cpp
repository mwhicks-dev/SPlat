#include "Client.h"
#include "Controller.h"

#include "events/KeyEvents.h"
#include "events/Listener.h"
#include "events/TickEvent.h"

#include <thread>
#include <chrono>

using namespace SPlat;

void Client::handle_key_event(sf::Keyboard::Key key) {
    if (sf::Keyboard::isKeyPressed(key)) {
        Events::KeyPressEvent press(key);
        press.raise();
    }
    else if (!sf::Keyboard::isKeyPressed(key)) {
        Events::KeyReleaseEvent release(key);
        release.raise();
    }
}

void Client::start() {
    std::pair<bool, std::mutex>& runtime = *new std::pair<bool, std::mutex>();
    runtime.first = true;

    std::thread t(&Controller::run, &ctl, std::ref(runtime));

    std::this_thread::sleep_for(std::chrono::milliseconds(250));  // give controller time to get started
    
    window.create(sf::VideoMode(800, 600), "SPlat");
    window.setFramerateLimit(60);

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

        // dispatch foreground events
        Events::ForegroundListener &lst = Events::ForegroundListener
            ::get_instance();
        std::thread t(&Events::Listener::run, &lst);
        t.detach();

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
}