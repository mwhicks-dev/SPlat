#include "events/Event.h"

using namespace SPlat::Events;

std::map<std::string, void (*)(std::string)> Event::handlers;
