#include "LocalTimeline.h"

#include <chrono>

#ifdef DEBUG
#include <iostream>
#endif

void SPlat::LocalTimeline::set_paused(bool paused) {
#ifdef DEBUG
    std::cout << "-> LocalTimeline::set_paused(" << paused << ")" << std::endl;
#endif
    m.lock();
    this->paused = paused;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- LocalTimeline::set_paused" << std::endl;
#endif
}

void SPlat::LocalTimeline::set_last_paused_time(time_t last_paused_time) {
#ifdef DEBUG
    std::cout << "-> LocalTimeline::set_last_paused_time(" << last_paused_time << ")" << std::endl;
#endif
    m.lock();
    this->last_paused_time = last_paused_time;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- LocalTimeline::set_last_paused_time" << std::endl;
#endif
}

time_t SPlat::LocalTimeline::get_last_paused_time() {
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
#ifdef DEBUG
    std::cout << "-> LocalTimeline::pause()" << std::endl;
#endif
    set_paused(true);
    set_last_paused_time(get_time());
#ifdef DEBUG
    std::cout << "<- LocalTimeline::pause" << std::endl;
#endif
}

void SPlat::LocalTimeline::unpause() {
#ifdef DEBUG
    std::cout << "-> LocalTimeline::unpause()" << std::endl;
#endif
    set_paused(false);
#ifdef DEBUG
    std::cout << "<- LocalTimeline::unpause" << std::endl;
#endif
}

void SPlat::LocalTimeline::set_tic(time_t tic) {
#ifdef DEBUG
    std::cout << "-> LocalTimeline::set_tic(" << tic << ")" << std::endl;
#endif
    m.lock();
    this->tic = tic;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- LocalTimeline::set_tic" << std::endl;
#endif
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
    time_t local = tic;
    m.unlock();

    return local;
}

time_t SPlat::LocalTimeline::get_elapsed_paused_time() {
    return get_paused()
        ? get_time() - get_last_paused_time()
        : 0;
}
