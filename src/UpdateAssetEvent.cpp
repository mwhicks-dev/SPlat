#include "events/UpdateAssetEvent.h"
#include "model/GameObjectModel.h"
#include "model/Character.h"
#include "model/Platform.h"
#include "model/MovingPlatform.h"

using namespace SPlat::Events;

std::string UpdateAssetEvent::TYPE = "update_asset";

void UpdateAssetEvent::handler(std::string serialized) {
    // deserialize event arguments
    UpdateAssetEventArgs args;
    std::stringstream ss; ss << serialized;
    {
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // create asset from args properties
    SPlat::Model::Asset * asset = nullptr;
    if (args.properties.type == SPlat::Model::Character::TYPE) {
        asset = new SPlat::Model::Character(args.properties.size);
    } else if (args.properties.type == SPlat::Model::Platform::TYPE) {
        asset = new SPlat::Model::Platform(args.properties.size);
    } else if (args.properties.type == SPlat::Model::MovingPlatform::TYPE) {
        asset = new SPlat::Model::MovingPlatform(args.properties.size);
    }
    if (asset == nullptr) {
        throw std::invalid_argument("No such asset type " 
            + args.properties.type);
    }
    asset->setPosition(args.properties.position);

    // update asset in GOM
    try {
        SPlat::Model::GameObjectModel::get_instance()
            .update_asset(args.id, *asset);
    } catch (std::invalid_argument& e) {
        delete asset;
        throw std::invalid_argument(e.what());
    }
    delete asset;
}