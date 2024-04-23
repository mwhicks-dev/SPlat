#include "events/AssetEvents.h"
#include "events/Command.h"
#include "events/Listener.h"
#include "model/GameObjectModel.h"
#include "Client.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

std::mutex ControlAssetEvent::control_lock;
bool ControlAssetEvent::control_set;
size_t ControlAssetEvent::control;

AddVelocityEvent::AddVelocityEvent(size_t id, sf::Vector2f velocity) {
    this->id = id;
    this->velocity = velocity;
}

void AddVelocityEvent::raise() {
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
}

void AddVelocityEvent::handler(std::string serialized) {
    // deserialize args from JSON string
    Args args;
    {
        std::stringstream ss;
        ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // get asset by ID
    SPlat::Model::Asset& asset = 
        SPlat::Model::GameObjectModel::get_instance()
        .read_asset(args.id);
    
    // add velocity to reference safely
    SPlat::Model::GameObjectModel::get_instance().lock.lock();
    asset.velocity += args.modifier;
    SPlat::Model::GameObjectModel::get_instance().lock.unlock();
}

AddPositionEvent::AddPositionEvent(size_t id, sf::Vector2f position) {
    this->id = id;
    this->position = position;
}

void AddPositionEvent::raise() {
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
}

void AddPositionEvent::handler(std::string serialized) {
    // deserialize args from JSON string
    Args args;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // get asset by ID
    SPlat::Model::Asset& asset = 
        SPlat::Model::GameObjectModel::get_instance().read_asset(args.id);
    
    // safely add position to asset
    SPlat::Model::GameObjectModel::get_instance().lock.lock();
    asset.move(args.modifier);
    size_t id = asset.id;
    SPlat::Model::GameObjectModel::get_instance().lock.unlock();

    SPlat::Model::GameObjectModel::get_instance().check_collision(id);
}

ControlAssetEvent::ControlAssetEvent(size_t id) { this->id = id; }

void ControlAssetEvent::raise() {
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
}

void ControlAssetEvent::handler(std::string serialized) {
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
