#include "model/Character.h"
#include "model/CharacterCollisionHandler.h"
#include "model/CharacterUpdateHandler.h"

using namespace SPlat::Model;

void Character::resolve_collision(Asset& other) {
    CollisionHandler * handler = get_collision_handler();

    if (handler == nullptr) {
        set_collision_handler(new CharacterCollisionHandler(
            get_asset_properties(), get_moving_properties(), 
            get_character_properties()));
    }

    handler->resolve_collision(other.get_asset_properties());
}

void Character::update() {
    UpdateHandler * handler = get_update_handler();

    if (handler == nullptr) {
        set_update_handler(new CharacterUpdateHandler(
            get_asset_properties(), get_moving_properties(), 
            get_character_properties()));
    }

    handler->update();
}
