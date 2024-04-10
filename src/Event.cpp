#include "Event.h"

std::map<std::string, void (*)(std::string)> SPlat::Event::handlers;
