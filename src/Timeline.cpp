#include "Timeline.h"

#include <chrono>

void SPlat::Timeline::set_paused(bool paused) {
    m.lock();
    this->paused = paused;
    m.unlock();
}

void SPlat::Timeline::set_last_paused_time(time_t last_paused_time) {
    m.lock();
    this->last_paused_time = last_paused_time;
    m.unlock();
}

SPlat::Timeline::Timeline() {
    this->tic = 1;
    paused = false;
}

SPlat::Timeline::Timeline(Timeline& anchor, time_t tic) {
    this->anchor = &anchor;
    this->tic = tic;
    paused = false;
    if (this->anchor != nullptr)
        start_time = this->anchor->get_time();
}

time_t SPlat::Timeline::get_time() {
    return (anchor->get_time() - get_start_time()) / get_tic();
}

void SPlat::Timeline::pause() {
    set_paused(true);
    set_last_paused_time(get_time());
}

void SPlat::Timeline::unpause() {
    set_paused(false);
}

void SPlat::Timeline::set_tic(time_t tic) {
    m.lock();
    this->tic = tic;
    m.unlock();
}

bool SPlat::Timeline::get_paused() {
    m.lock();
    bool local = paused;
    m.unlock();

    return local;
}

time_t SPlat::Timeline::get_start_time() {
    m.lock();
    time_t local = start_time;
    m.unlock();

    return local;
}

time_t SPlat::Timeline::get_tic() {
    m.lock();
    time_t local = start_time;
    m.unlock();

    return local;
}

time_t SPlat::SystemTimeline::get_time() {
    return std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()
    );
}
