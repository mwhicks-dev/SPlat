#include "Environment.h"

using namespace SPlat;

Environment::Environment() {
    set_running(true);
}

bool Environment::get_running() {
    m.lock();
    bool local = running;
    m.unlock();

    return running;
}

void Environment::set_running(bool running) {
    m.lock();
    this->running = running;
    m.unlock();
}
