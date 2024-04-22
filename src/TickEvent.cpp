#include "events/TickEvent.h"
#include "events/AssetEvents.h"

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
            // validate asset
            SPlat::Model::GameObjectModel::get_instance().validate(id);

            // get and update asset by ID
            SPlat::Model::Asset& curr = SPlat::Model::GameObjectModel
                ::get_instance().read_asset(id);
            curr.update();
        } catch (std::exception& e) {/* OK */}
    }
}