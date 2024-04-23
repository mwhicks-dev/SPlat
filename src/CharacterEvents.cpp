#include "events/CharacterEvents.h"
#include "events/AssetEvents.h"
#include "events/Command.h"
#include "events/Listener.h"

#include "model/AssetFactory.h"
#include "model/Character.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Events;

CreateCharacterEvent::CreateCharacterEvent
        (SPlat::Model::AssetProperties properties) {
    this->properties = properties;
}

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

SPlat::Model::Character from_properties
        (SPlat::Model::AssetProperties properties) {
    // create character template
    SPlat::Model::Character tmp(properties.size);
    tmp.setPosition(properties.position);

    // pass to asset factory and update
    SPlat::Model::Character c 
        = SPlat::Model::AssetFactory<SPlat::Model::Character>
          ::create_asset(tmp);
    
    return c;
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
    std::cout << "<- CreateControlCharacterEvent::raise()" << std::endl;
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
    ControlAssetEvent e(c.id); e.raise();
#ifdef DEBUG
    std::cout << "<- CreateControlCharacterEvent::handler" << std::endl;
#endif
}
