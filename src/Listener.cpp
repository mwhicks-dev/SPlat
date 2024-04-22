#include "events/Listener.h"

using namespace SPlat::Events;

void Listener::run() {
    while (true) {
        // break if no events to process
        events_lock.lock();
        if (events.empty()) {
            events_lock.unlock();
            break;
        }
        // dequeue event if nonempty
        Event event = events.top(); events.pop();
        events_lock.unlock();

        // dispatch event
        event.dispatch();
    }
}

void Listener::push_event(Event event) {
    events_lock.lock();
    events.push(event);
    events_lock.unlock();
}