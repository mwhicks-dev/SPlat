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
            // validate asset
            SPlat::Model::GameObjectModel::get_instance().validate(id);

            // get asset by ID
            SPlat::Model::Asset& curr = SPlat::Model::GameObjectModel
                ::get_instance().read_asset(id);

            // raise update asset event with velocity
            SPlat::Model::AssetProperties prop = curr.get_properties();
            prop.position += curr.velocity;
            UpdateAssetEvent event = curr.get_priority() >= 0 && curr.standing_on == nullptr
                ? UpdateAssetEvent(curr.id, prop, sf::Vector2f(0, 1.5))
                : UpdateAssetEvent(curr.id, prop);
            event.raise();
        } catch (std::exception& e) {/* OK */}
    }
}