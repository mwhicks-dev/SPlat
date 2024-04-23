#include "events/TickEvent.h"
#include "events/Command.h"
#include "events/Listener.h"
#include "model/GameObjectModel.h"

#include <cereal/archives/json.hpp>

#include <iostream>

using namespace SPlat::Events;

TickEvent::TickEvent() {
    ids = SPlat::Model::GameObjectModel::get_instance().getIds();
}

void TickEvent::raise() {
    // serialize args to JSON string
    Args args = {.ids=ids};
    std::stringstream ss;
    {
        cereal::JSONOutputArchive oar(ss);
        oar(args);
    }

    // create new command
    Command cmd = {
        .priority=1,
        .type=get_type(),
        .args=ss.str()
    };

    // send to background listener
    BackgroundListener::get_instance().push_command(cmd);
}

void TickEvent::handler(std::string serialized) {
    // deserialize args
    Args args;
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