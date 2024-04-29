#include "Runtime.h"
#include "model/CharacterFactory.h"
#include "model/PlatformFactory.h"
#include "model/MovingPlatformFactory.h"

#include <thread>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat;

Runtime * Runtime::instance = nullptr;
std::mutex Runtime::m_static;

class SystemTimeline : public Timeline {

    time_t get_time() override {
        return std::chrono::system_clock::now().time_since_epoch().count();
    }

};

Runtime::Runtime(Timeline& anchor)
: anchor(anchor), display_timeline(anchor, 1), 
  character_factory(*new Model::CharacterFactory()),
  platform_factory(*new Model::PlatformFactory()),
  moving_platform_factory(*new Model::MovingPlatformFactory()) {
    update_anchor_steps_per_second();
    display_timeline.set_tic(get_anchor_steps_per_second());
    set_running(true);
}

bool Runtime::get_running() {
    m.lock();
    bool local = running;
    m.unlock();

     return local;
}

void Runtime::set_running(bool running) {
#ifdef DEBUG
    std::cout << "-> Runtime::set_running(" << running << ")" << std::endl;
#endif
    m.lock();
    this->running = running;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- Runtime::set_running" << std::endl;
#endif
}

Timeline& Runtime::get_anchor_timeline() {
    m.lock();
    Timeline& local = anchor;
    m.unlock();

    return local;
}

void Runtime::set_anchor_timeline(Timeline& anchor) {
#ifdef DEBUG
    std::cout << "-> Runtime::set_anchor_timeline(Timeline&)" << std::endl;
#endif
    m.lock();
    this->anchor = anchor;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- Runtime::set_anchor_timeline" << std::endl;
#endif
}

void Runtime::set_anchor_steps_per_second(time_t anchor_steps_per_second) {
#ifdef DEBUG
    std::cout << "-> Runtime::set_anchor_steps_per_second(" << anchor_steps_per_second << ")" << std::endl;
#endif
    m.lock();
    this->anchor_steps_per_second = anchor_steps_per_second;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- Runtime::set_anchor_steps_per_second" << std::endl;
#endif
}

void Runtime::update_anchor_steps_per_second() {
#ifdef DEBUG
    std::cout << "-> Runtime::update_anchor_steps_per_second()" << std::endl;
#endif
    time_t t0 = get_anchor_timeline().get_time();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    time_t tf = get_anchor_timeline().get_time();

    set_anchor_steps_per_second(tf - t0);
#ifdef DEBUG
    std::cout << "<- Runtime::update_anchor_steps_per_second" << std::endl;
#endif
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

void Runtime::update_anchor_timeline(Timeline& anchor) {
#ifdef DEBUG
    std::cout << "-> Runtime::update_anchor_timeline(Timeline&)" << std::endl;
#endif
    bool running = get_running();
    if (running)
        throw std::invalid_argument("Cannot update anchor timeline during run");
    
    set_anchor_timeline(anchor);
    update_anchor_steps_per_second();
#ifdef DEBUG
    std::cout << "<- Runtime::update_anchor_timeline" << std::endl;
#endif
}

Model::AbstractAssetFactory& Runtime::get_character_factory() {
    m.lock();
    Model::AbstractAssetFactory& local = character_factory;
    m.unlock();

    return local;
}

Model::AbstractAssetFactory& Runtime::get_platform_factory() {
    m.lock();
    Model::AbstractAssetFactory& local = platform_factory;
    m.unlock();
    
    return local;
}

Model::AbstractAssetFactory& Runtime::get_moving_platform_factory() {
    m.lock();
    Model::AbstractAssetFactory& local = moving_platform_factory;
    m.unlock();
    
    return local;
}

time_t Runtime::get_anchor_steps_per_second() {
    m.lock();
    time_t local = anchor_steps_per_second;
    m.unlock();

    return local;
}
