#include "events/CharacterEvents.h"
#include "events/AssetEvents.h"
#include "events/Command.h"
#include "events/Listener.h"

#include "model/AssetFactory.h"
#include "model/Character.h"

using namespace SPlat::Events;

CreateCharacterEvent::CreateCharacterEvent
        (SPlat::Model::AssetProperties properties) {
    this->properties = properties;
}

void CreateCharacterEvent::raise() {
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
    // deserialize args from JSON string
    Args args;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // create new character from passed properties
    from_properties(args.properties);
}

void CreateControlCharacterEvent::raise() {
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
}

void CreateControlCharacterEvent::handler(std::string serialized) {
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
}
