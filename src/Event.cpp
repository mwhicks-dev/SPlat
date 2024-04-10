#include "Event.h"

std::map<std::string, void (*)(std::string)> SPlat::Event::handlers;

void SPlat::Event::set_handler(std::string type, void (*func)(std::string)) {
    // override existing/defaults OK
    handlers[type] = func;
}
