#include "events/CharacterEvents.h"

#include "events/ControlAssetEvent.h"

#include "model/AssetFactory.h"
#include "model/Character.h"

using namespace SPlat::Events;

std::string CreateCharacterEvent::TYPE = "create_character_event";

void CreateCharacterEvent::handler(std::string serialized) {
    CreateCharacterEvent::Args args;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    SPlat::Model::Character c(args.properties.size);
    c.setPosition(args.properties.position);

    SPlat::Model::AssetFactory<SPlat::Model::Character>::create_asset(c);
}

std::string CreateControlCharacterEvent::TYPE = "create_control_character_event";

void CreateControlCharacterEvent::handler(std::string serialized) {
    CreateControlCharacterEvent::Args args;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    SPlat::Model::Character c(args.properties.size);
    c.setPosition(args.properties.position);

    c = SPlat::Model::AssetFactory<SPlat::Model::Character>::create_asset(c);

    ControlAssetEvent ctl(c.id);
    ctl.raise();
}