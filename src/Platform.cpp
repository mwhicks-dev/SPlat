#include "model/Platform.h"

using namespace SPlat::Model;

sf::Color rand_platform_color() {
    int r = 48 + rand() % 80;
    int g = 48 + rand() % 80;
    int b = 48 + rand() % 80;
    return sf::Color(r, g, b);
}

Platform::Platform(sf::Vector2f& size) 
: Asset(size, rand_platform_color()) {}

int Platform::get_priority() { return -2; }

std::string Platform::TYPE = "platform";

std::string Platform::get_type() { return Platform::TYPE; }
