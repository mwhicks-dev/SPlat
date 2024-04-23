#include "model/Character.h"

using namespace SPlat::Model;

sf::Color rand_character_color() {
    std::srand(time(0));
    int r = 168 + rand() % 44;
    int g = 168 + rand() % 44;
    int b = 168 + rand() % 44;
    return sf::Color(r, g, b);
}

Character::Character(sf::Vector2f& size)
: Asset(size, rand_character_color()) {}

int Character::get_priority() { return 0; }

std::string Character::TYPE = "character";

std::string Character::get_type() { return Character::TYPE; }
