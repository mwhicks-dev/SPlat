#include "events/CreateAssetEvent.h"
#include "utilities/Functions.h"

using namespace SPlat::Events;

std::string CreateAssetEvent::TYPE = "create_asset";

/// @brief function executed on create asset event dispatch
/// @param serialized arguments passed as serialized string
/// @throws if args cannot be deserialized or asset type invalid
void CreateAssetEvent::handler(std::string serialized) {
    // create new asset using GOM
    SPlat::Utilities::deserialize_asset(serialized);
}
