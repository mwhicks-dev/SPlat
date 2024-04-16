#include "events/UpdateAssetEvent.h"
#include "utilities/Functions.h"

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

    // reserialize asset properties
    ss = std::stringstream();
    {
        cereal::JSONOutputArchive oar(ss);
        oar(args.properties);
    }

    // create asset from args properties
    SPlat::Model::Asset& asset = 
        SPlat::Utilities::deserialize_asset(ss.str());
    asset.velocity += args.velocity_modifier;

    // update asset in GOM
    try {
        SPlat::Model::GameObjectModel::get_instance()
            .update_asset(args.id, asset);
        delete &asset;
    } catch (std::invalid_argument& e) {
        delete &asset;
        throw std::invalid_argument(e.what());
    }
}