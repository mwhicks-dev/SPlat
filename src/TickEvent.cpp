#include "events/TickEvent.h"
#include "events/UpdateAssetEvent.h"

#include <exception>
#include <cmath>

using namespace SPlat::Events;

std::string TickEvent::TYPE = "tick_event";

void TickEvent::handler(std::string serialized) {
    // deserialize args
    TickEventArgs args;
    std::stringstream ss; ss << serialized;
    {
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // for each asset...
    for (size_t id : args.ids) {
        try {
            // get asset by ID
            SPlat::Model::Asset& curr = SPlat::Model::GameObjectModel
                ::get_instance().read_asset(id);

            // raise update asset event with velocity
            SPlat::Model::AssetProperties prop = curr.get_properties();
            prop.position += curr.velocity;
            UpdateAssetEvent event(curr.id, prop);
        } catch (std::exception& e) {/* OK */}
    }
}