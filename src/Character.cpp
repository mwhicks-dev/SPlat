#include "model/Character.h"

using namespace SPlat::Model;

sf::Color rand_character_color() {
    srand(time(0));
    int r = 128 + rand() % 128;
    int g = 128 + rand() % 128;
    int b = 128 + rand() % 128;
    return sf::Color(r, g, b);
}

Character::Character(sf::Vector2f& size)
: Asset(size, rand_character_color()) {}

int Character::get_priority() { return 0; }

std::string Character::TYPE = "character";

std::string Character::get_type() { return Character::TYPE; }
