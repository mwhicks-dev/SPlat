#include "events/MovingPlatformEvents.h"

#include "model/AssetFactory.h"

using namespace SPlat::Events;

std::string CreateMovingPlatformEvent::TYPE = "create_moving_platform_event";

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
