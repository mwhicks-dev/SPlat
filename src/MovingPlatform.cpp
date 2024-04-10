#include "model/MovingPlatform.h"

using namespace SPlat::Model;

MovingPlatform::MovingPlatform(sf::Vector2f& size)
: Platform(size) {}

int MovingPlatform::get_priority() { return -1; }

std::string MovingPlatform::TYPE = "moving_platform";
