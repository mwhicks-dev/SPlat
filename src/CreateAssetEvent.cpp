#include "events/CreateAssetEvent.h"
#include "utilities/Functions.h"

using namespace SPlat::Events;

std::string CreateAssetEvent::TYPE = "create_asset";

/// @brief function executed on create asset event dispatch
/// @param serialized arguments passed as serialized string
/// @throws if args cannot be deserialized or asset type invalid
void CreateAssetEvent::handler(std::string serialized) {
    // create new asset from properties
    SPlat::Model::Asset& asset 
        = SPlat::Utilities::deserialize_asset(serialized);
    
    // add new asset to GOM
    SPlat::Model::GameObjectModel::get_instance()
        .create_asset(asset);
}
