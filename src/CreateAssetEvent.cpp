#include "events/CreateAssetEvent.h"
#include "model/Character.h"
#include "model/Platform.h"
#include "model/MovingPlatform.h"
#include "model/GameObjectModel.h"

using namespace SPlat::Events;

std::string CreateAssetEvent::CREATE_ASSET_EVENT_TAG = "create_asset";

/// @brief function executed on create asset event dispatch
/// @param serialized arguments passed as serialized string
/// @throws if args cannot be deserialized or asset type invalid
void CreateAssetEvent::handler(std::string serialized) {
    // deserialize args from string
    CreateAssetEventArgs args;
    std::stringstream ss; ss << serialized;
    {
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // create new asset using GOM
    SPlat::Model::Asset * created_asset;
    if (args.type == SPlat::Model::Character::TYPE) {
        created_asset = new SPlat::Model::Character(args.size);
    } else if (args.type == SPlat::Model::Platform::TYPE) {
        created_asset = new SPlat::Model::Platform(args.size);
    } else if (args.type == SPlat::Model::MovingPlatform::TYPE) {
        created_asset = new SPlat::Model::MovingPlatform(args.size);
    } else {
        throw std::domain_error("No such asset type " + args.type);
    }

    created_asset->setPosition(args.position);
    SPlat::Model::GameObjectModel::get_instance().create_asset(*created_asset);
}
