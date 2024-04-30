#include "events/TickEvent.h"
#include "events/Command.h"
#include "events/Listener.h"
#include "model/GameObjectModel.h"
#include "model/Moving.h"
#include "Config.h"

#include <cereal/archives/json.hpp>

#include <vector>
#include <algorithm>

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

bool compare(SPlat::Model::Moving* lhs, SPlat::Model::Moving* rhs) {
    return lhs->get_asset_properties().get_collision_priority()
        < rhs->get_asset_properties().get_collision_priority();
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
    std::vector<SPlat::Model::Moving*> moving;
    for (size_t id : args.ids) {
        try {
            SPlat::Model::Moving& asset = dynamic_cast<SPlat::Model::Moving&>(
                SPlat::Model::GameObjectModel::get_instance().read_asset(id));
            moving.push_back(&asset);
        } catch (std::bad_cast&) {}
    }
    sort(moving.begin(), moving.end(), compare);
    time_t curr = Config::get_instance().get_timing_config()
        .get_display_timeline().get_time();
    for (SPlat::Model::Moving* asset_ptr : moving) {
        try {
            // get and update
            SPlat::Model::Moving& asset = *asset_ptr;
            asset.update();
            asset.get_moving_properties().set_last_update(curr);

            size_t id = asset.get_asset_properties().get_id();
            for (size_t other : args.ids) {
                if (id == other) continue;

                SPlat::Model::Asset& other_asset = Model::GameObjectModel::get_instance().read_asset(other);
                asset.resolve_collision(other_asset);
            }
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