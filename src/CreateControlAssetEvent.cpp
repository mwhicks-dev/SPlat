#include "events/CreateControlAssetEvent.h"
#include "events/ControlAssetEvent.h"
#include "utilities/Functions.h"

using namespace SPlat::Events;

std::string CreateControlAssetEvent::TYPE = "create_control_asset";

void CreateControlAssetEvent::handler(std::string serialized) {
    SPlat::Model::Asset& asset = SPlat::Utilities::deserialize_asset(serialized);
    
    // set asset to be controleld
    ForegroundListener& lst = ForegroundListener::get_instance();
    lst.push_event(ControlAssetEvent(asset.id));
}
