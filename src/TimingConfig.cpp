#include "TimingConfig.h"
#include "Client.h"

#include <chrono>
#include <thread>

#ifdef DEBUG
#include <iostream>
#endif

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
    std::thread t(&TimingConfig::display_timeline_loop, this);
    t.detach();
}

void TimingConfig::set_anchor_steps_per_second(
        time_t anchor_steps_per_second) {
#ifdef DEBUG
    std::cout << "-> TimingConfig::set_anchor_steps_per_second(" << anchor_steps_per_second << ")" << std::endl;
#endif
    const std::lock_guard<std::mutex> lock(m);
    this->anchor_steps_per_second = anchor_steps_per_second;
#ifdef DEBUG
    std::cout << "<- TimingConfig::set_anchor_steps_per_second" << std::endl;
#endif
}

Timeline& TimingConfig::get_anchor_timeline() {
    const std::lock_guard<std::mutex> lock(m);
    return anchor_timeline;
}

LocalTimeline& TimingConfig::get_display_timeline() {
    const std::lock_guard<std::mutex> lock(m);
    return display_timeline;
}

time_t TimingConfig::get_anchor_steps_per_second() {
    const std::lock_guard<std::mutex> lock(m);
    return anchor_steps_per_second;
}

long TimingConfig::get_framerate_limit() {
    const std::lock_guard<std::mutex> lock(m);
    return framerate_limit;
}

void TimingConfig::set_framerate_limit(long framerate_limit) {
#ifdef DEBUG
    std::cout << "-> TimingConfig::set_framerate_limit(" << framerate_limit << ")" << std::endl;
#endif
    const std::lock_guard<std::mutex> lock(m);
    this->framerate_limit = framerate_limit;
#ifdef DEBUG
    std::cout << "<- TimingConfig::set_framerate_limit" << std::endl;
#endif
}

void TimingConfig::update_framerate_limit(long framerate_limit) {
#ifdef DEBUG
    std::cout << "-> TimingConfig::update_framerate_limit(" << framerate_limit << ")" << std::endl;
#endif
    set_framerate_limit(framerate_limit);
    get_display_timeline().set_tic(get_anchor_steps_per_second() / framerate_limit);
#ifdef DEBUG
    std::cout << "<- TimingConfig::update_framerate_limit(" << std::endl;
#endif
}

void TimingConfig::display_timeline_loop() {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    EnvironmentInterface& environment 
        = Entrypoint::get_instance().get_config().get_environment();
    Timeline& anchor_timeline = get_anchor_timeline();
    LocalTimeline& display_timeline = get_display_timeline();

    try {
        Client::get_instance();  // see if running client or not
    } catch (std::exception&) { return; }

    time_t initial = anchor_timeline.get_time();

    time_t seconds_passed = 0;
    while (environment.get_running()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        time_t current = anchor_timeline.get_time();
        seconds_passed += 1;
        time_t average = (current - initial) / seconds_passed;
        set_anchor_steps_per_second(average);
        display_timeline.set_tic(get_anchor_steps_per_second() / get_framerate_limit());
    }
}
