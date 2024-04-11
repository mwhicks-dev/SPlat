#include "Client.h"
#include "Controller.h"

#include "events/KeyEvents.h"

#include <thread>
#include <iostream>

using namespace SPlat;

void Client::start() {
    window.create(sf::VideoMode(800, 600), "SPlat");
    std::pair<bool, std::mutex>& runtime = *new std::pair<bool, std::mutex>();
    runtime.first = true;

    Controller ctl;  // single controller
    std::thread t(&Controller::run, &ctl, std::ref(runtime));

    while (window.isOpen()) {
        // poll for closed event
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // check for keyboard inputs
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !Events::KeyEvent::is_key_held(sf::Keyboard::Left)) {
            ctl.push_event(Events::KeyPressEvent(sf::Keyboard::Left));
            std::cout << "left key press" << std::endl;
        } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && Events::KeyEvent::is_key_held(sf::Keyboard::Left)) {
            ctl.push_event(Events::KeyReleaseEvent(sf::Keyboard::Left));
            std::cout << "left key release" << std::endl;
        }

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