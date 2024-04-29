#include "TimingConfig.h"

#include <chrono>
#include <thread>

using namespace SPlat;

class SystemTimeline : public Timeline {

    time_t get_time() override {
        return std::chrono::system_clock::now().time_since_epoch().count();
    }

};

TimingConfig::TimingConfig()
: anchor_timeline(*new SystemTimeline()), 
display_timeline(*new LocalTimeline(anchor_timeline, -1)) {
    time_t t0 = get_anchor_timeline().get_time();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    time_t tf = get_anchor_timeline().get_time();
    set_anchor_steps_per_second(tf - t0);
    display_timeline.set_tic(get_anchor_steps_per_second());
}

void TimingConfig::set_anchor_steps_per_second(
        time_t anchor_steps_per_second) {
    m.lock();
    this->anchor_steps_per_second = anchor_steps_per_second;
    m.unlock();
}

Timeline& TimingConfig::get_anchor_timeline() {
    m.lock();
    Timeline& local = anchor_timeline;
    m.unlock();

    return local;
}

LocalTimeline& TimingConfig::get_display_timeline() {
    m.lock();
    LocalTimeline& local = display_timeline;
    m.unlock();

    return local;
}

time_t TimingConfig::get_anchor_steps_per_second() {
    m.lock();
    time_t local = anchor_steps_per_second;
    m.unlock();

    return local;
}
