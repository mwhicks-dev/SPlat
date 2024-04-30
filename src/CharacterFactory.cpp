#include "model/CharacterFactory.h"
#include "model/Character.h"
#include "model/GameObjectModel.h"
#include "Client.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

Asset& CharacterFactory::create_asset(AssetProperties& properties) {
    // create new moving and character properties
    properties.set_collision_priority(0);
    MovingProperties moving_properties(sf::Vector2f(0, 0), 
        Client::get_instance().get_config().get_timing_config().get_display_timeline().get_time());
    CharacterProperties character_properties;

    Character * character = new Character(
        *new AssetProperties(properties),
        *new MovingProperties(moving_properties),
        *new CharacterProperties(character_properties)
    );

    return GameObjectModel::get_instance().create_asset(*character);
}
