#include "model/Platform.h"
#include "model/handler/PlatformCollisionHandler.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

void Platform::resolve_collision(Asset& other) {
#ifdef DEBUG
    std::cout << "-> Platform::resolve_collision(Asset&)" << std::endl;
#endif
    if (get_collision_handler() == nullptr)
        set_collision_handler(new PlatformCollisionHandler(get_asset_properties()));

    get_collision_handler()->resolve_collision(other.get_asset_properties());
#ifdef DEBUG
    std::cout << "<- Platform::resolve_collision" << std::endl;
#endif
}