#include "events/MovingPlatformEvents.h"
#include "events/Command.h"
#include "events/Listener.h"
#include "model/AssetFactory.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

CreateMovingPlatformEvent::CreateMovingPlatformEvent(
    SPlat::Model::AssetProperties properties,
    std::vector<SPlat::Model::MovingPlatform::State> states
) {
    this->properties = properties;
    this->states = states;
}

void CreateMovingPlatformEvent::raise() {
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
}

void CreateMovingPlatformEvent::handler(std::string serialized) {
    CreateMovingPlatformEvent::Args args;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    SPlat::Model::MovingPlatform mplat(args.properties.size);
    mplat.setPosition(args.properties.position);

    mplat = SPlat::Model::AssetFactory<SPlat::Model::MovingPlatform>
        ::create_asset(mplat);
    
    SPlat::Model::MovingPlatform& persistent = (SPlat::Model::MovingPlatform&)
        SPlat::Model::GameObjectModel::get_instance().read_asset(mplat.id);

    for (SPlat::Model::MovingPlatform::State s : args.states) {
        SPlat::Model::GameObjectModel::get_instance().lock.lock();
        persistent.add_state(s);
        SPlat::Model::GameObjectModel::get_instance().lock.unlock();
    }
}
