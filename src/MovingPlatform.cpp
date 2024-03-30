#include "MovingPlatform.h"

namespace SPlat {

    MovingPlatform::MovingPlatform(sf::Vector2f& size)
    : Platform(size) {}

    int MovingPlatform::get_priority() { return -1; }

}