#include "events/AssetEvents.h"
#include "events/Command.h"
#include "events/Listener.h"
#include "model/GameObjectModel.h"
#include "model/Moving.h"
#include "Client.h"
#include "Config.h"

#include <cereal/archives/json.hpp>

#include <sstream>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Events;

std::mutex ControlAssetEvent::control_lock;
bool ControlAssetEvent::control_set;
size_t ControlAssetEvent::control;

AddVelocityEvent::AddVelocityEvent(size_t id, sf::Vector2f velocity) {
    this->id = id;
    this->velocity = velocity;
}

void AddVelocityEvent::raise() {
#ifdef DEBUG
    std::cout << "-> AddVelocityEvent::raise()" << std::endl;
#endif
    // serialize args to JSON string
    Args args = {.id=id, .modifier=velocity};
    std::stringstream ss;
    {
        cereal::JSONOutputArchive oar(ss);
        oar(args);
    }

    // create new command 
    Command cmd= {
        .type=AddVelocityEvent::get_type(),
        .args=ss.str()
    };

    // send command to background listener
    BackgroundListener::get_instance().push_command(cmd);
#ifdef DEBUG
    std::cout << "<- AddVelocityEvent::raise" << std::endl;
#endif
}

void AddVelocityEvent::handler(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> AddVelocityEvent::handler(" << serialized << ")" << std::endl;
#endif
    // deserialize args from JSON string
    Args args;
    {
        std::stringstream ss;
        ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // get asset by ID
    SPlat::Model::Moving& asset = (SPlat::Model::Moving&)
        SPlat::Model::GameObjectModel::get_instance()
        .read_asset(args.id);
    
    // add velocity to reference safely
    SPlat::Model::MovingProperties& properties = asset.get_moving_properties();
    properties.set_velocity(asset.get_moving_properties().get_velocity() + args.modifier);
    properties.set_last_update(Config::get_instance().get_timing_config()
        .get_display_timeline().get_time());
#ifdef DEBUG
    std::cout << "<- AddVelocityEvent::handler" << std::endl;
#endif
}

AddPositionEvent::AddPositionEvent(size_t id, sf::Vector2f position) {
    this->id = id;
    this->position = position;
}

void AddPositionEvent::raise() {
#ifdef DEBUG
    std::cout << "-> AddVelocityEvent::raise()" << std::endl;
#endif
    // serialize args to JSON string
    Args args = {.id=id, .modifier=position};
    std::stringstream ss;
    {
        cereal::JSONOutputArchive oar(ss);
        oar(args);
    }

    // create new command 
    Command cmd = {
        .type=AddPositionEvent::get_type(),
        .args=ss.str()
    };

    // send command to background listener
    BackgroundListener::get_instance().push_command(cmd);
#ifdef DEBUG
    std::cout << "<- AddVelocityEvent::raise" << std::endl;
#endif
}

void AddPositionEvent::handler(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> AddPositionEvent::handler(" << serialized << ")" << std::endl;
#endif
    // deserialize args from JSON string
    Args args;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // get asset by ID
    SPlat::Model::Moving& asset = (SPlat::Model::Moving&)
        SPlat::Model::GameObjectModel::get_instance().read_asset(args.id);
    
    // safely add position to asset
    SPlat::Model::MovingProperties& properties = asset.get_moving_properties();
    properties.set_position(properties.get_position() + args.modifier);
    properties.set_last_update(Config::get_instance().get_timing_config()
        .get_display_timeline().get_time());
    size_t id = properties.get_id();

    SPlat::Model::GameObjectModel::get_instance().check_collision(id);
#ifdef DEBUG
    std::cout << "<- AddPositionEvent::handler" << std::endl;
#endif
}

ControlAssetEvent::ControlAssetEvent(size_t id) { this->id = id; }

void ControlAssetEvent::raise() {
#ifdef DEBUG
    std::cout << "-> ControlAssetEvent::raise()" << std::endl;
#endif
    // serialize args to JSON string
    Args args = {.id=id};
    std::stringstream ss;
    {
        cereal::JSONOutputArchive oar(ss);
        oar(args);
    }

    // create new command
    Command cmd = {
        .priority=-2,
        .type=ControlAssetEvent::get_type(),
        .args=ss.str()
    };

    // send to foreground listener
    ForegroundListener::get_instance().push_command(cmd);
#ifdef DEBUG
    std::cout << "<- ControlAssetEvent::raise" << std::endl;
#endif
}

void ControlAssetEvent::handler(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> ControlAssetEvent::handler(" << serialized << ")" << std::endl;
#endif
    // deserialize args from JSON string
    Args args;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // verify asset matching ID exists
    SPlat::Model::Asset& controlled =
        SPlat::Model::GameObjectModel::get_instance().read_asset(args.id);
    
    // set asset as controlled by ID
    control_lock.lock();
    control_set = true;
    control = args.id;
    control_lock.unlock();
#ifdef DEBUG
    std::cout << "<- ControlAssetEvent::handler" << std::endl;
#endif
}

size_t ControlAssetEvent::get_controlled_asset_id() {
    // get static values
    size_t local_control;
    bool local_control_set = false;
    control_lock.lock();
    local_control_set = control_set;
    if (local_control_set)
        local_control = control;
    control_lock.unlock();

    // return by local values
    if (!local_control_set)
        throw std::domain_error("No asset set as controlled");
    
    return local_control;
}
