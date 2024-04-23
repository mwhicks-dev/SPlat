#include <thread>

#include "Controller.h"
#include "events/Listener.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat;

std::mutex Controller::events_lock;
std::queue<Events::Event> Controller::events;

void Controller::run(std::pair<bool, std::mutex>& runtime) {
#ifdef DEBUG
    std::cout << "-> Controller::run(std::pair<bool, std::mutex>&)" << std::endl;
#endif
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
#ifdef DEBUG
    std::cout << "<- Controller::run" << std::endl;
#endif
}