#include "Environment.h"

using namespace SPlat;

Environment::Environment() {
    set_running(true);
    set_framerate_limit(60);
}

long Environment::get_framerate_limit() {
    m.lock();
    long local = framerate_limit;
    m.unlock();

    return local;
}

bool Environment::get_running() {
    m.lock();
    bool local = running;
    m.unlock();

    return local;
}

float Environment::get_unit() {
    m.lock();
    float local = unit;
    m.unlock();

    return local;
}

void Environment::set_framerate_limit(long framerate_limit) {
    m.lock();
    this->framerate_limit = framerate_limit;
    m.unlock();
}

void Environment::set_running(bool running) {
    m.lock();
    this->running = running;
    m.unlock();
}

void Environment::set_unit(float unit) {
    m.lock();
    this->unit = unit;
    m.unlock();
}
