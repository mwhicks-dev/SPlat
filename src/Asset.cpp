#include "model/Asset.h"
#include "events/AssetEvents.h"

using namespace SPlat::Model;

Asset::Asset(sf::Vector2f& size) {
    this->setSize(size);
}

void Asset::update() {
    // raise relevant update events
    {  // velocity update
        SPlat::Events::Event event 
            = SPlat::Events::AddPositionEvent(id, velocity);
        event.raise();
    }
    if (get_priority() >= 0) {  // gravity update
        SPlat::Events::Event event 
            = SPlat::Events::AddVelocityEvent(id, sf::Vector2f(0, 1.5));
        event.raise();
    }
}