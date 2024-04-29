#include "events/TickEvent.h"
#include "events/Command.h"
#include "events/Listener.h"
#include "model/GameObjectModel.h"
#include "model/Moving.h"
#include "Runtime.h"

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
    time_t curr = Runtime::get_instance().get_display_timeline().get_time();
    for (size_t id : args.ids) {
        try {
            // get and update
            SPlat::Model::Moving& asset = (SPlat::Model::Moving&) SPlat::Model
                ::GameObjectModel::get_instance().read_asset(id);
            asset.update(curr);
            asset.get_moving_properties().set_last_update(curr);
        } catch (std::exception& e) {
#ifdef DEBUG
            std::cout << e.what() << std::endl;
#endif
        }
    }
#ifdef DEBUG
    std::cout << "<- TickEvent::handler" << std::endl;
#endif
}