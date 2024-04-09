#include "Event.h"

namespace SPlat {

    Event::Event(std::string type, std::string args) {
        this->type = type;
        this->args = args;
    }

    Event::dispatch() {
        // throw iae
        if (handlers.count(type) == 0)
            throw std::domain_error("No such event type " + type);
        
        handlers[type](args);
    }

    Event::add_handler(std::string type, void (*func)(std::string)) {
        // override existing/defaults OK
        handlers[type] = func;
    }

}