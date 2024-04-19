#include "events/AssetEvents.h"
#include "model/GameObjectModel.h"
#include "model/AssetFactory.h"
#include "model/Character.h"
#include "model/Platform.h"
#include "model/MovingPlatform.h"

using namespace SPlat::Events;

std::string AddVelocityEvent::TYPE = "add_asset_velocity";

std::string AddPositionEvent::TYPE = "add_position_event";

void add_velocity_helper(SPlat::Model::Asset& asset, sf::Vector2f& velocity) {
    asset.velocity += velocity;
}

void AddVelocityEvent::handler(std::string serialized) {
    // get args back from string
    AddVelocityEvent::Args args;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // get asset ref from GOM
    SPlat::Model::Asset& ref = 
        SPlat::Model::GameObjectModel::get_instance().read_asset(args.id);
    
    //std::cout << "Asset " << ref.id << " initial velocity: {x:" << ref.velocity.x << ", y:" << ref.velocity.y << "}" << std::endl;

    // depending on type of asset, get and update value
    if (ref.get_type() == SPlat::Model::Character::TYPE) {  // character
        SPlat::Model::Character val = SPlat::Model
            ::AssetFactory<SPlat::Model::Character>::read_asset(args.id);
        add_velocity_helper(val, args.modifier);
        SPlat::Model::AssetFactory<SPlat::Model::Character>::update_asset(args
            .id, val);
    } else if (ref.get_type() == SPlat::Model::Platform::TYPE) {  // platform
        SPlat::Model::Platform val = SPlat::Model
            ::AssetFactory<SPlat::Model::Platform>::read_asset(args.id);
        add_velocity_helper(val, args.modifier);
        SPlat::Model::AssetFactory<SPlat::Model::Platform>::update_asset(args
            .id, val);
    } else if (ref.get_type() == SPlat::Model::MovingPlatform::TYPE) {  // moving platform
        SPlat::Model::MovingPlatform val = SPlat::Model
            ::AssetFactory<SPlat::Model::MovingPlatform>::read_asset(args.id);
        add_velocity_helper(val, args.modifier);
        SPlat::Model::AssetFactory<SPlat::Model::MovingPlatform>
            ::update_asset(args.id, val);
    }

    //std::cout << "Asset " << ref.id << " final velocity: {x:" << ref.velocity.x << ", y:" << ref.velocity.y << "}" << std::endl;
}

void add_position_helper(SPlat::Model::Asset& asset, sf::Vector2f& update) {
    sf::Vector2f position = asset.getPosition();
    position += update;
    asset.setPosition(position);
}

void AddPositionEvent::handler(std::string serialized) {
    // get args back from string
    AddPositionEvent::Args args;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // get asset ref from GOM
    SPlat::Model::Asset& ref = 
        SPlat::Model::GameObjectModel::get_instance().read_asset(args.id);

    //std::cout << "Asset " << ref.id << " initial position: {x:" << ref.getPosition().x << ", y:" << ref.getPosition().y << "}" << std::endl;
    
    // depending on type of asset, get and update value
    if (ref.get_type() == SPlat::Model::Character::TYPE) {  // character
        SPlat::Model::Character val = SPlat::Model
            ::AssetFactory<SPlat::Model::Character>::read_asset(args.id);
        add_position_helper(val, args.modifier);
        SPlat::Model::AssetFactory<SPlat::Model::Character>::update_asset(args
            .id, val);
    } else if (ref.get_type() == SPlat::Model::Platform::TYPE) {  // platform
        SPlat::Model::Platform val = SPlat::Model
            ::AssetFactory<SPlat::Model::Platform>::read_asset(args.id);
        add_position_helper(val, args.modifier);
        SPlat::Model::AssetFactory<SPlat::Model::Platform>::update_asset(args
            .id, val);
    } else if (ref.get_type() == SPlat::Model::MovingPlatform::TYPE) {  // moving platform
        SPlat::Model::MovingPlatform val = SPlat::Model
            ::AssetFactory<SPlat::Model::MovingPlatform>::read_asset(args.id);
        add_position_helper(val, args.modifier);
        SPlat::Model::AssetFactory<SPlat::Model::MovingPlatform>
            ::update_asset(args.id, val);
    }

    //std::cout << "Asset " << ref.id << " final position: {x:" << ref.getPosition().x << ", y:" << ref.getPosition().y << "}" << std::endl;
}