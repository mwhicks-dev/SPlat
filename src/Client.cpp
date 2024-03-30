#include "Client.h"

namespace SPlat {

    void Client::start() {
        window.create(sf::VideoMode(800, 600), "SPlat");

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
            }

            window.clear(sf::Color::Black);

            std::unordered_set<size_t> asset_ids = GameObjectModel::
                get_instance().getIds();
            for (size_t id : asset_ids) {
                Asset& asset = GameObjectModel::get_instance().read_asset(id);
                window.draw(asset);
            }

            window.display();
        }
    }

}