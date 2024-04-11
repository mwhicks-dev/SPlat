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
        events_lock.lock();
        Events::Event event = events.front(); events.pop();
        void (*handler)(std::string) = Events::Event::handlers[event.type];
        events_lock.unlock();

        // process event
        handler(event.args);
    }
}

void Controller::push_event(Events::Event event) {
    events_lock.lock();
    events.push(event);
    events_lock.unlock();
}