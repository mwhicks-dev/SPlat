#include "events/TickEvent.h"

#include <exception>

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
            
            // TODO: raise update asset event with velocity
            sf::Vector2f updated_position = curr.getPosition();
            updated_position.x += curr.velocity.x;
            updated_position.y += curr.velocity.y;
            curr.setPosition(updated_position);

            SPlat::Model::GameObjectModel
                ::get_instance().update_asset(id, curr);
        } catch (std::exception& e) {/* OK */}
    }
}