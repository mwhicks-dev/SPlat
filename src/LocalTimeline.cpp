#include "LocalTimeline.h"

#include <chrono>

void SPlat::LocalTimeline::set_paused(bool paused) {
    m.lock();
    this->paused = paused;
    m.unlock();
}

void SPlat::LocalTimeline::set_last_paused_time(time_t last_paused_time) {
    m.lock();
    this->last_paused_time = last_paused_time;
    m.unlock();
}

time_t SPlat::LocalTimeline::get_elapsed_paused_time() {
    m.lock();
    time_t local = last_paused_time;
    m.unlock();

    return local;
}

SPlat::LocalTimeline::LocalTimeline(Timeline& anchor, time_t tic)
: anchor(anchor), start_time(anchor.get_time()) {
    set_tic(tic);
    paused = false;
}

time_t SPlat::LocalTimeline::get_time() {
    return (anchor.get_time() - get_start_time()) / get_tic();
}

void SPlat::LocalTimeline::pause() {
    set_paused(true);
    set_last_paused_time(get_time());
}

void SPlat::LocalTimeline::unpause() {
    set_paused(false);
}

void SPlat::LocalTimeline::set_tic(time_t tic) {
    m.lock();
    this->tic = tic;
    m.unlock();
}

bool SPlat::LocalTimeline::get_paused() {
    m.lock();
    bool local = paused;
    m.unlock();

    return local;
}

time_t SPlat::LocalTimeline::get_start_time() {
    m.lock();
    time_t local = start_time;
    m.unlock();

    return local;
}

time_t SPlat::LocalTimeline::get_tic() {
    m.lock();
    time_t local = start_time;
    m.unlock();

    return local;
}

time_t SPlat::LocalTimeline::get_elapsed_paused_time() {
    return get_paused()
        ? get_time() - get_last_paused_time()
        : 0;
}
