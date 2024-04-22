#include "model/Platform.h"

using namespace SPlat::Model;

Platform::Platform(sf::Vector2f& size) 
: Asset(size) {}

int Platform::get_priority() { return -2; }

std::string Platform::TYPE = "platform";

std::string Platform::get_type() { return Platform::TYPE; }
