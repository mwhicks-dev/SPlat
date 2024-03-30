#include "Character.h"

namespace SPlat {

    Character::Character(sf::Vector2f& size)
    : Asset(size) {}

    int Character::get_priority() { return 0; }

}