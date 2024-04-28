#include <thread>

#include "Controller.h"
#include "Runtime.h"
#include "events/Listener.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat;

std::mutex Controller::events_lock;
std::queue<Events::Event> Controller::events;

void Controller::run() {
#ifdef DEBUG
    std::cout << "-> Controller::run()" << std::endl;
#endif
    while (true) {
        // check if still running
        if (!Runtime::get_instance().get_running()) break;

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