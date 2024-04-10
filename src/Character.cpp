#include "model/Character.h"

using namespace SPlat::Model;

Character::Character(sf::Vector2f& size)
: Asset(size) {}

int Character::get_priority() { return 0; }

std::string Character::TYPE = "character";
