#include "model/Platform.h"

using namespace SPlat::Model;

sf::Color rand_platform_color() {
    std::srand(time(0));
    int r = 136 + rand() % 32;
    int g = 136 + rand() % 32;
    int b = 136 + rand() % 32;
    return sf::Color(r, g, b);
}

Platform::Platform(sf::Vector2f& size) 
: Asset(size, rand_platform_color()) {}

int Platform::get_priority() { return -2; }

std::string Platform::TYPE = "platform";

std::string Platform::get_type() { return Platform::TYPE; }
