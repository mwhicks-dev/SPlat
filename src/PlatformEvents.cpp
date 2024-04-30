#include "events/PlatformEvents.h"
#include "events/Command.h"
#include "events/Listener.h"
#include "model/Platform.h"
#include "Client.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

CreatePlatformEvent::CreatePlatformEvent
        (SPlat::Model::AssetProperties properties) 
        : properties(properties) {}

void CreatePlatformEvent::raise() {
#ifdef DEBUG
    std::cout << "-> CreatePlatformEvent::raise()" << std::endl;
#endif
    // serialize args to JSON string
    Args args = {.properties=properties};
    std::stringstream ss;
    {
        cereal::JSONOutputArchive oar(ss);
        oar(args);
    }

    // create new command
    Command cmd = {
        .type=CreatePlatformEvent::get_type(),
        .args=ss.str()
    };

    // send to background listener
    BackgroundListener::get_instance().push_command(cmd);
#ifdef DEBUG
    std::cout << "<- CreatePlatformEvent::raise" << std::endl;
#endif
}

void CreatePlatformEvent::handler(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> CreatePlatformEvent::raise(" << serialized << ")" << std::endl;
#endif
    // deserialize args from JSON string
    Args args;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // create platfrom from args
    Client::get_instance().get_config().get_asset_factory_config()
        .get_platform_factory().create_asset(args.properties);
#ifdef DEBUG
    std::cout << "<- CreatePlatformEvent::handler" << std::endl;
#endif
}
