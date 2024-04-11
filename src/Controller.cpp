#include "Controller.h"

using namespace SPlat;

std::mutex Controller::events_lock;
std::queue<Events::Event> Controller::events;

void Controller::run(std::pair<bool&, std::mutex> runtime) {
    while (true) {
        // check if client still running
        runtime.second.lock();
        if (!runtime.first) {
            runtime.second.unlock();
            break;
        }
        runtime.second.unlock();

        // get event and handler
        bool has_event = false;
        Events::Event event;
        void (*handler)(std::string);
        events_lock.lock();
        if (events.size() > 0) {
            event = events.front(); events.pop();
            handler = Events::Event::handlers[event.type];
            has_event = true;
        }
        events_lock.unlock();

        // process event
        if (has_event)
            handler(event.args);
    }
}

void Controller::push_event(Events::Event event) {
    events_lock.lock();
    events.push(event);
    events_lock.unlock();
}