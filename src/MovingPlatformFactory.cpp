#include "Runtime.h"
#include "model/GameObjectModel.h"
#include "model/MovingPlatformFactory.h"
#include "model/MovingPlatform.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

Asset& MovingPlatformFactory::create_asset(AssetProperties& properties) {
    MovingPlatform * mplat = new MovingPlatform(
        (MovingPlatformProperties&) properties,
        *new DefaultCollisionHandler(),
        *new DefaultUpdateHandler()
    );
    return GameObjectModel::get_instance().create_asset(*mplat);
}

Asset& MovingPlatformFactory::update_asset(size_t id, 
        AssetProperties& properties) {
    MovingPlatform& mplat 
        = (MovingPlatform&) GameObjectModel::get_instance().read_asset(id);
    
    MovingPlatformProperties& curr 
        = (MovingPlatformProperties&) mplat.get_moving_properties();
    curr.set_fill_color(properties.get_fill_color());
    curr.set_position(properties.get_position());
    curr.set_size(properties.get_size());

    try {
        {
            MovingProperties& other = (MovingProperties&) properties;
            curr.set_last_update(other.get_last_updated());
            curr.set_velocity(other.get_velocity());
        }
        {
            MovingPlatformProperties& other 
                = (MovingPlatformProperties&) properties;
            curr.set_states(other.get_states());
            curr.set_last_state_change(other.get_last_state_change());
        }
    } catch (std::exception&) {/* OK */}

    return mplat;
}

void MovingPlatformFactory::DefaultUpdateHandler::update(time_t curr) {
#ifdef DEBUG
    std::cout << "-> MovingPlatformFactory::DefaultUpdateHandler::update(" << curr << ")" << std::endl;
#endif
    MovingPlatformProperties& properties 
        = (MovingPlatformProperties&) *get_properties();

    std::vector<State> states = properties.get_states();
    if (states.size() > 0) {
        State curr_state = states[0];
        properties.set_velocity(curr_state.velocity);
    }

    AbstractMovingFactory::DefaultUpdateHandler initial;
    initial.set_properties(get_properties());
    initial.update(curr);
    
    if (states.size() > 0) {
        State curr_state = states[0];
        if (Runtime::get_instance().get_anchor_steps_per_second() * (curr 
                - properties.get_last_state_change()) < curr_state
                .anchor_steps) return;
        
        states.erase(states.begin());
        if (curr_state.repeat)
            states.push_back(curr_state);
        properties.set_states(states);
    }
#ifdef DEBUG
    std::cout << "<- MovingPlatformFactory::DefaultUpdateHandler::update" << std::endl;
#endif
}