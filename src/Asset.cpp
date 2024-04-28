#include "model/Asset.h"
#include "events/AssetEvents.h"
#include "Runtime.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

Asset::Asset(sf::Vector2f& size) {
    this->setSize(size);
    this->last_updated = Runtime::get_instance().get_anchor_timeline().get_time();
}

Asset::Asset(sf::Vector2f& size, sf::Color fill_color) {
    setSize(size);
    setFillColor(fill_color);
    this->last_updated = Runtime::get_instance().get_anchor_timeline().get_time();
}

void Asset::update() {
#ifdef DEBUG
    std::cout << "-> Asset::update()" << std::endl;
#endif
    // raise relevant update events
    time_t local = last_updated;
    {  // velocity update
        sf::Vector2f update_velocity = velocity * static_cast<float>(
            Runtime::get_instance().get_anchor_timeline().get_time() - 
            local) / static_cast<float>(Runtime::get_instance()
            .get_anchor_steps_per_second()) * (float)(!Runtime::get_instance()
            .get_display_timeline().get_paused());
        SPlat::Events::AddPositionEvent event(id, update_velocity);
        event.raise();
        for (size_t stander_id : standers) {
            event = SPlat::Events::AddPositionEvent(stander_id, update_velocity);
            event.raise();
        }
    }
    if (get_priority() >= 0 && standing_on == nullptr) {  // gravity update
        sf::Vector2f update_velocity = sf::Vector2f(0, 490) 
            * static_cast<float>(Runtime::get_instance().get_anchor_timeline()
            .get_time() - local) / static_cast<float>(Runtime::get_instance()
            .get_anchor_steps_per_second()) * (float)(!Runtime::get_instance()
            .get_display_timeline().get_paused());
        SPlat::Events::AddVelocityEvent event(id, update_velocity);
        event.raise();
    }
#ifdef DEBUG
    std::cout << "<- Asset::update" << std::endl;
#endif
}