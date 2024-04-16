#include "events/Event.h"
#include "events/Listener.h"

using namespace SPlat::Events;

std::map<std::string, void (*)(std::string)> Event::handlers;
std::mutex Event::handlers_lock;

void Event::raise() {
    Listener * lst;
    if (this->foreground) {  // foreground event
        lst = &ForegroundListener::get_instance();
    } else {  // background event
        lst = &BackgroundListener::get_instance();
    }

    lst->push_event(*this);
}
