#include "model/Asset.h"
#include "events/AssetEvents.h"

using namespace SPlat::Model;

Asset::Asset(sf::Vector2f& size) {
    this->setSize(size);
}

void Asset::update() {
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
}