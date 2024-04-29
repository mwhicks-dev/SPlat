#include "events/CharacterEvents.h"
#include "events/AssetEvents.h"
#include "events/Command.h"
#include "events/Listener.h"
#include "model/Character.h"
#include "Config.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Events;

CreateCharacterEvent::CreateCharacterEvent
    (SPlat::Model::MovingProperties properties) : properties(properties) {}

void CreateCharacterEvent::raise() {
#ifdef DEBUG
    std::cout << "-> CreateCharacterEvent::raise()" << std::endl;
#endif
    // serialize args to JSON string
    Args args = {.properties=properties};
    std::stringstream ss;
    {
        cereal::JSONOutputArchive oar(ss);
        oar(args);
    }

    // create new command
    Command cmd = {
        .type=CreateCharacterEvent::get_type(),
        .args=ss.str()
    };

    // send to background listener
    BackgroundListener::get_instance().push_command(cmd);
#ifdef DEBUG
    std::cout << "<- CreateCharacterEvent::raise" << std::endl;
#endif
}

SPlat::Model::Character& from_properties
        (SPlat::Model::AssetProperties properties) {
    // pass to asset factory and update
    return (SPlat::Model::Character&) SPlat::Config::get_instance()
        .get_asset_factory_config().get_character_factory().create_asset(properties);
}

void CreateCharacterEvent::handler(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> CreateCharacterEvent::handler(" << serialized << ")" << std::endl;
#endif
    // deserialize args from JSON string
    Args args;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // create new character from passed properties
    from_properties(args.properties);
#ifdef DEBUG
    std::cout << "<- CreateCharacterEvent::handler" << std::endl;
#endif
}

void CreateControlCharacterEvent::raise() {
#ifdef DEBUG
    std::cout << "-> CreateControlCharacterEvent::raise()" << std::endl;
#endif
    // serialize args to JSON string
    Args args = {.properties=properties};
    std::stringstream ss;
    {
        cereal::JSONOutputArchive oar(ss);
        oar(args);
    }

    // create new command
    Command cmd = {
        .type=CreateControlCharacterEvent::get_type(),
        .args=ss.str()
    };

    // send to background listener
    BackgroundListener::get_instance().push_command(cmd);
#ifdef DEBUG
    std::cout << "<- CreateControlCharacterEvent::raise" << std::endl;
#endif
}

void CreateControlCharacterEvent::handler(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> CreateControlCharacterEvent::handler(" << serialized << ")" << std::endl;
#endif
    // deserialize args from JSON string
    Args args;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // create new character from passed properties
    SPlat::Model::Character c = from_properties(args.properties);

    // create, raise new ControlAssetEvent
    ControlAssetEvent e(c.get_asset_properties().get_id()); e.raise();
#ifdef DEBUG
    std::cout << "<- CreateControlCharacterEvent::handler" << std::endl;
#endif
}
