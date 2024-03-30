#include "Platform.h"

namespace SPlat {

    Platform::Platform(sf::Vector2f& size) 
    : Asset(size) {}

    int Platform::get_priority() { return -2; }

}