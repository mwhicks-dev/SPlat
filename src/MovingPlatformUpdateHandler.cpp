#include "model/handler/MovingPlatformUpdateHandler.h"
#include "events/Command.h"
#include "events/handlers/ClientUpdateAssetHandler.h"
#include "Entrypoint.h"
#include "Event.h"

#include <cereal/archives/json.hpp>

#include <string>


#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

AssetProperties& MovingPlatformUpdateHandler::get_asset_properties() {
    const std::lock_guard<std::mutex> lock(m);
    return asset;
}

MovingProperties& MovingPlatformUpdateHandler::get_moving_properties() {
    const std::lock_guard<std::mutex> lock(m);
    return moving;
}

MovingPlatformProperties& MovingPlatformUpdateHandler::get_moving_platform_properties() {
    const std::lock_guard<std::mutex> lock(m);
    return moving_platform;
}

void MovingPlatformUpdateHandler::update() {
#ifdef DEBUG
    std::cout << "-> MovingPlatformHandler::update()" << std::endl;
#endif
    ConfigInterface& conf = Entrypoint::get_instance().get_config();
    AssetProperties& asset_properties = get_asset_properties();
    MovingProperties& moving_properties = get_moving_properties();
    MovingPlatformProperties& moving_platform_properties = get_moving_platform_properties();

    // check if state change needed
    {
        std::vector<State> states = moving_platform_properties.get_states();
        if (states.size() > 0 && conf.get_timing_config().get_anchor_timeline()
                .get_time() > moving_platform_properties
                .get_last_state_change() + states[0].anchor_steps) {
            if (states[0].repeat) states.push_back(states[0]);
            states.erase(states.begin());
            moving_platform_properties.set_last_state_change(conf
                .get_timing_config().get_anchor_timeline().get_time());
        }
        moving_platform_properties.set_states(states);
    }
    
    // set velocity to current state velocity
    {
        std::vector<State> states = moving_platform_properties.get_states();
        sf::Vector2f velocity(0, 0);
        if (states.size() > 0) {
            velocity = states[0].velocity;
        }
        moving_properties.set_velocity(velocity);
    }

    // get velocity
    sf::Vector2f update_velocity;
    {
        float dt = static_cast<float>(conf.get_timing_config()
            .get_display_timeline().get_time() - moving_properties
            .get_last_updated()) / static_cast<float>(conf.get_timing_config()
            .get_framerate_limit());
        update_velocity = moving_properties.get_velocity() * dt;
    }

    // update self and children
    {
        asset_properties.set_position(asset_properties.get_position() + update_velocity);
        conf.get_environment().get_standing_config().push_update_to_children(
            asset_properties.get_id(), update_velocity);
    }

    asset_properties.set_updated_time(conf.get_timing_config().get_anchor_timeline().get_time());

    // raise update event for persistence
    Events::ClientUpdateAssetHandler::Args args = {
        .id=asset_properties.get_id(),
        .properties=asset_properties
    };
    std::stringstream ss;
    {
        cereal::JSONOutputArchive oar(ss);
        oar(args);
    }
    Events::Command cmd = {
        .priority=0,
        .type=Events::ClientUpdateAssetHandler::get_type(),
        .args=ss.str(),
    };
    Event event = {
        .command=cmd,
        .sender=conf.get_environment().get_entrypoint_id(),
    };
    ss.clear(); ss.str("");
    {
        cereal::JSONOutputArchive oar(ss);
        oar(event);
    }
    Request request = {
        .content_type=Request::ContentType::Event,
        .body=ss.str()
    };
    Entrypoint::get_instance().get_controller().push_outgoing_request(request);
#ifdef DEBUG
    std::cout << "<- MovingPlatformHandler::update()" << std::endl;
#endif
}
