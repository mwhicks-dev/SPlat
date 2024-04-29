#include "model/MovingPlatformUpdateHandler.h"
#include "Config.h"

using namespace SPlat::Model;

AssetProperties& MovingPlatformUpdateHandler::get_asset_properties() {
    m.lock();
    AssetProperties& local = asset;
    m.unlock();

    return local;
}

MovingProperties& MovingPlatformUpdateHandler::get_moving_properties() {
    m.lock();
    MovingProperties& local = moving;
    m.unlock();

    return local;
}

MovingPlatformProperties& MovingPlatformUpdateHandler::get_moving_platform_properties() {
    m.lock();
    MovingPlatformProperties& local = moving_platform;
    m.unlock();

    return local;
}

void MovingPlatformUpdateHandler::update() {
    Config& conf = Config::get_instance();

    // check if state change needed
    MovingPlatformProperties& moving_platform_properties = get_moving_platform_properties();
    std::vector<State> states = moving_platform_properties.get_states();

    // check if state to manipulate
    if (states.size() == 0) return;

    if (Config::get_instance().get_timing_config().get_anchor_timeline().get_time() >
            moving_platform_properties.get_last_state_change() + states[0].anchor_steps) {
        State popped = states[0];
        states.erase(states.begin());
        if (popped.repeat) states.push_back(popped);
        moving_platform.set_states(states);
    }

    // check again if state to manipulate
    if (states.size() == 0) return;

    // set velocity to first state velocity
    MovingProperties& moving_properties = get_moving_properties();
    sf::Vector2f velocity = states[0].velocity;
    moving_properties.set_velocity(velocity);

    // move according to velocity
    AssetProperties& asset_properties = get_asset_properties();
    asset_properties.set_position(asset_properties.get_position() + velocity * 
        static_cast<float>(conf.get_timing_config().get_display_timeline()
        .get_time() - moving_properties.get_last_updated()) / 
        static_cast<float>(conf.get_environment().get_framerate_limit()));
}
