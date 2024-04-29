#include "model/MovingPlatform.h"
#include "model/MovingPlatformCollisionHandler.h"
#include "model/MovingPlatformUpdateHandler.h"

using namespace SPlat::Model;

void MovingPlatform::resolve_collision(Asset& other) {
    CollisionHandler * handler = get_collision_handler();

    if (handler == nullptr) {
        set_collision_handler(new MovingPlatformCollisionHandler(
            get_asset_properties(), get_moving_properties(), 
            get_moving_platform_properties()));
    }

    handler->resolve_collision(other.get_asset_properties());
}

void MovingPlatform::update() {
    UpdateHandler * handler = get_update_handler();

    if (handler == nullptr) {
        set_update_handler(new MovingPlatformUpdateHandler(
            get_asset_properties(), get_moving_properties(), 
            get_moving_platform_properties()));
    }

    handler->update();
}
