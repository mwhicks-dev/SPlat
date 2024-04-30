#include "model/Platform.h"
#include "model/handler/PlatformCollisionHandler.h"

using namespace SPlat::Model;

void Platform::resolve_collision(Asset& other) {
    if (get_collision_handler() == nullptr)
        set_collision_handler(new PlatformCollisionHandler(get_asset_properties()));

    get_collision_handler()->resolve_collision(other.get_asset_properties());
}