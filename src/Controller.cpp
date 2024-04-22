#include <thread>

#include "Controller.h"
#include "events/Listener.h"

using namespace SPlat;

std::mutex Controller::events_lock;
std::queue<Events::Event> Controller::events;

void Controller::run(std::pair<bool, std::mutex>& runtime) {
    while (true) {
        // check if client still running
        runtime.second.lock();
        if (!runtime.first) {
            runtime.second.unlock();
            break;
        }
        runtime.second.unlock();

        // dispatch background events
        Events::BackgroundListener &lst = Events::BackgroundListener
            ::get_instance();
        std::thread t(&Events::Listener::run, &lst);
        t.detach();
    }
}