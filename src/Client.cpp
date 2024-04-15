#include "Client.h"
#include "Controller.h"

#include "events/KeyEvents.h"
#include "events/TickEvent.h"

#include <thread>

using namespace SPlat;

void Client::handle_key_event(sf::Keyboard::Key key) {
    if (sf::Keyboard::isKeyPressed(key) && !Events::KeyEvent::is_key_held(key)) {
        Events::KeyPressEvent event(key);
    } else if (!sf::Keyboard::isKeyPressed(key) && Events::KeyEvent::is_key_held(key)) {
        Events::KeyReleaseEvent event(key);
    }
}

void Client::start() {
    window.create(sf::VideoMode(800, 600), "SPlat");
    window.setFramerateLimit(60);
    std::pair<bool, std::mutex>& runtime = *new std::pair<bool, std::mutex>();
    runtime.first = true;

    std::thread t(&Controller::run, &ctl, std::ref(runtime));

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