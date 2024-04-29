#include "model/CharacterUpdateHandler.h"
#include "Config.h"

using namespace SPlat::Model;

AssetProperties& CharacterUpdateHandler::get_asset_properties() {
    m.lock();
    AssetProperties& local = asset;
    m.unlock();

    return local;
}

MovingProperties& CharacterUpdateHandler::get_moving_properties() {
    m.lock();
    MovingProperties& local = moving;
    m.unlock();

    return local;
}

CharacterProperties& CharacterUpdateHandler::get_character_properties() {
    m.lock();
    CharacterProperties& local = character;
    m.unlock();

    return local;
}

void CharacterUpdateHandler::update() {
    Config& conf = Config::get_instance();

    // move according to velocity
    AssetProperties& asset_properties = get_asset_properties();
    MovingProperties& moving_properties = get_moving_properties();
    sf::Vector2f velocity = moving_properties.get_velocity();
    asset_properties.set_position(asset_properties.get_position() + velocity * 
        static_cast<float>(conf.get_timing_config().get_display_timeline()
        .get_time() - moving_properties.get_last_updated()) / 
        static_cast<float>(conf.get_environment().get_framerate_limit()));
    
    // if not standing on anything, increment y velocity
    if (get_character_properties().get_standing_on() == nullptr) {
        EnvironmentInterface& env = conf.get_environment();
        velocity.y += 10. * env.get_unit() 
            / static_cast<float>(env.get_framerate_limit());
    }
}
