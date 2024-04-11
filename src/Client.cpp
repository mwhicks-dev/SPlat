#include "Client.h"
#include "Controller.h"

#include <thread>

using namespace SPlat;

void Client::start() {
    window.create(sf::VideoMode(800, 600), "SPlat");
    std::pair<bool, std::mutex> runtime;
    runtime.first = true;

    Controller ctl;  // single controller
    std::thread t(&Controller::run, &ctl, std::ref(runtime));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

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