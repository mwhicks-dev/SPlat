#include "events/AssetEvents.h"
#include "events/Command.h"
#include "events/Listener.h"
#include "model/GameObjectModel.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

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
        .type=get_type(),
        .args=ss.str()
    };

    // send command to backend listener
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
        .type=get_type(),
        .args=ss.str()
    };

    // send command to backend listener
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
    SPlat::Model::GameObjectModel::get_instance().lock.unlock();
}
