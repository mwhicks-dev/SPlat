#include "events/MovingPlatformEvents.h"
#include "events/Command.h"
#include "events/Listener.h"
#include "Config.h"

#include <cereal/archives/json.hpp>

#include <sstream>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Events;

CreateMovingPlatformEvent::CreateMovingPlatformEvent(
    SPlat::Model::AssetProperties properties,
    std::vector<SPlat::Model::State> states
) : properties(properties) {
    this->states = states;
}

void CreateMovingPlatformEvent::raise() {
#ifdef DEBUG
    std::cout << "-> CreateMovingPlatformEvent::raise()" << std::endl;
#endif
    // serialize args to JSON string
    Args args = {.properties=properties, .states=states};
    std::stringstream ss;
    {
        cereal::JSONOutputArchive oar(ss);
        oar(args);
    }

    // create new command
    Command cmd = {
        .type=CreateMovingPlatformEvent::get_type(),
        .args=ss.str()
    };

    // send to background listener
    BackgroundListener::get_instance().push_command(cmd);
#ifdef DEBUG
    std::cout << "<- CreateMovingPlatformEvent::raise" << std::endl;
#endif
}

void CreateMovingPlatformEvent::handler(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> CreateMovingPlatformEvent::handler(" << serialized << ")" << std::endl;
#endif
    CreateMovingPlatformEvent::Args args;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    Config::get_instance().get_asset_factory_config()
        .get_moving_platform_factory().create_asset(args.properties);
#ifdef DEBUG
    std::cout << "<- CreateMovingPlatformEvent::handler" << std::endl;
#endif
}
