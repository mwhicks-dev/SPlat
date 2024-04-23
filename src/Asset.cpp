#include "model/Asset.h"
#include "events/AssetEvents.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

Asset::Asset(sf::Vector2f& size) {
    this->setSize(size);
}

Asset::Asset(sf::Vector2f& size, sf::Color fill_color) {
    setSize(size);
    setFillColor(fill_color);
}

void Asset::update() {
#ifdef DEBUG
    std::cout << "-> Asset::update()" << std::endl;
#endif
    // raise relevant update events
    {  // velocity update
        SPlat::Events::AddPositionEvent event(id, velocity);
        event.raise();
        for (size_t stander_id : standers) {
            event = SPlat::Events::AddPositionEvent(stander_id, velocity);
            event.raise();
        }
    }
    if (get_priority() >= 0 && standing_on == nullptr) {  // gravity update
        SPlat::Events::AddVelocityEvent event(id, sf::Vector2f(0, 1.5));
        event.raise();
    }
#ifdef DEBUG
    std::cout << "<- Asset::update" << std::endl;
#endif
}