#include "Runtime.h"

using namespace SPlat;

Runtime * Runtime::instance = nullptr;
std::mutex Runtime::m_static;

class SystemTimeline : public Timeline {

    time_t get_time() override {
        return std::chrono::system_clock::now().time_since_epoch().count();
    }

};

Runtime::Runtime(Timeline& anchor)
: anchor(anchor), display_timeline(anchor, 1) {
    set_running(true);
}

bool Runtime::get_running() {
    m.lock();
    bool local = running;
    m.unlock();

     return local;
}

void Runtime::set_running(bool running) {
    m.lock();
    this->running = running;
    m.unlock();
}

Timeline& Runtime::get_anchor_timeline() {
    m.lock();
    Timeline& local = anchor;
    m.unlock();

    return local;
}

void Runtime::set_anchor_timeline(Timeline& anchor) {
    m.lock();
    this->anchor = anchor;
    m.unlock();
}

LocalTimeline& Runtime::get_display_timeline() {
    m.lock();
    LocalTimeline& local = display_timeline;
    m.unlock();

    return local;
}

Runtime& Runtime::get_instance() {
    m_static.lock();
    if (instance == nullptr)
        instance = new Runtime(*new SystemTimeline());
    m_static.unlock();
    return *instance;
}