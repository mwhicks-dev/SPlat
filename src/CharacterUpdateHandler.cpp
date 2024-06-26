#include "model/handler/CharacterUpdateHandler.h"
#include "Entrypoint.h"
#include "Event.h"

#include <cereal/archives/json.hpp>

#include <string>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

AssetProperties& CharacterUpdateHandler::get_asset_properties() {
    const std::lock_guard<std::mutex> lock(m);
    return asset;
}

MovingProperties& CharacterUpdateHandler::get_moving_properties() {
    const std::lock_guard<std::mutex> lock(m);
    return moving;
}

CharacterProperties& CharacterUpdateHandler::get_character_properties() {
    const std::lock_guard<std::mutex> lock(m);
    return character;
}

void CharacterUpdateHandler::update() {
#ifdef DEBUG
    std::cout << "-> CharacterUpdateHandler::update()" << std::endl;
#endif
    ConfigInterface& conf = Entrypoint::get_instance().get_config();
    AssetProperties& asset_properties = get_asset_properties();
    MovingProperties& moving_properties = get_moving_properties();
    CharacterProperties& character_properties = get_character_properties();

    // move according to velocity
    sf::Vector2f velocity = moving_properties.get_velocity();
    float dt = static_cast<float>(conf.get_timing_config().get_display_timeline()
        .get_time() - moving_properties.get_last_updated()) / 
        static_cast<float>(conf.get_timing_config().get_framerate_limit());
    asset_properties.set_position(asset_properties.get_position() + velocity * dt);

    asset_properties.set_updated_time(conf.get_timing_config().get_anchor_timeline().get_time());

    // raise update event for persistence

    AssetProperties * standing_on = character_properties.get_standing_on();    
    // if not standing on anything, increment y velocity
    if (get_character_properties().get_standing_on() == nullptr) {
        EnvironmentInterface& env = conf.get_environment();
        velocity.y += 10. * env.get_unit() * dt;
        moving_properties.set_velocity(velocity);
    } else {
        // otherwise, check if still standing on thing
        sf::RectangleShape cpy = asset_properties.get_rectangle_shape();
        cpy.move(0, 1);
        if (!cpy.getGlobalBounds().intersects(standing_on
                ->get_rectangle_shape().getGlobalBounds())) {
            conf.get_environment().get_standing_config().remove_child(
                standing_on->get_id(), asset_properties.get_id());
            character_properties.set_standing_on(nullptr);
        }
    }
#ifdef DEBUG
    std::cout << "<- CharacterUpdateHandler::update" << std::endl;
#endif
}
