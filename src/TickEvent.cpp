#include "events/TickEvent.h"
#include "events/Command.h"
#include "events/Listener.h"
#include "model/GameObjectModel.h"

#include <cereal/archives/json.hpp>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Events;

TickEvent::TickEvent() {
    ids = SPlat::Model::GameObjectModel::get_instance().getIds();
}

void TickEvent::raise() {
#ifdef DEBUG
    std::cout << "-> TickEvent::raise()" << std::endl;
#endif
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
        .type=TickEvent::get_type(),
        .args=ss.str()
    };

    // send to background listener
    BackgroundListener::get_instance().push_command(cmd);
#ifdef DEBUG
    std::cout << "<- TickEvent::raise" << std::endl;
#endif
}

void TickEvent::handler(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> TickEvent::handler(" << serialized << ")" << std::endl;
#endif
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
#ifdef DEBUG
    std::cout << "<- TickEvent::handler" << std::endl;
#endif
}