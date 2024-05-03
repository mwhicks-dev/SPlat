#include "model/Character.h"
#include "model/handler/CharacterCollisionHandler.h"
#include "model/handler/CharacterUpdateHandler.h"

using namespace SPlat::Model;

void Character::resolve_collision(Asset& other) {
    if (get_collision_handler() == nullptr) {
        set_collision_handler(new CharacterCollisionHandler(
            get_asset_properties(), get_moving_properties(), 
            get_character_properties()));
    }

    get_collision_handler()->resolve_collision(other.get_asset_properties());
}

void Character::update() {
    if (get_update_handler() == nullptr) {
        set_update_handler(new CharacterUpdateHandler(
            get_asset_properties(), get_moving_properties(), 
            get_character_properties()));
    }

    get_update_handler()->update();
}
