#include "model/MovingPlatform.h"
#include "model/handler/MovingPlatformCollisionHandler.h"
#include "model/handler/MovingPlatformUpdateHandler.h"

using namespace SPlat::Model;

void MovingPlatform::resolve_collision(Asset& other) {
    if (get_collision_handler() == nullptr) {
        set_collision_handler(new MovingPlatformCollisionHandler(
            get_asset_properties(), get_moving_properties(), 
            get_moving_platform_properties()));
    }

    get_collision_handler()->resolve_collision(other.get_asset_properties());
}

void MovingPlatform::update() {
    if (get_update_handler() == nullptr) {
        set_update_handler(new MovingPlatformUpdateHandler(
            get_asset_properties(), get_moving_properties(), 
            get_moving_platform_properties()));
    }

    get_update_handler()->update();
}
