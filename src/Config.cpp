#include "Config.h"
#include "AssetFactoryConfig.h"

#include <thread>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat;

Config * Config::instance = nullptr;
std::mutex Config::m_static;

class SystemTimeline : public Timeline {

    time_t get_time() override {
        return std::chrono::system_clock::now().time_since_epoch().count();
    }

};

Config::Config(Timeline& anchor)
: anchor(anchor), display_timeline(anchor, 1), 
  asset_factory_config(*new AssetFactoryConfig()) {
    update_anchor_steps_per_second();
    display_timeline.set_tic(get_anchor_steps_per_second());
    set_running(true);
}

bool Config::get_running() {
    m.lock();
    bool local = running;
    m.unlock();

     return local;
}

void Config::set_running(bool running) {
#ifdef DEBUG
    std::cout << "-> Config::set_running(" << running << ")" << std::endl;
#endif
    m.lock();
    this->running = running;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- Config::set_running" << std::endl;
#endif
}

Timeline& Config::get_anchor_timeline() {
    m.lock();
    Timeline& local = anchor;
    m.unlock();

    return local;
}

void Config::set_anchor_timeline(Timeline& anchor) {
#ifdef DEBUG
    std::cout << "-> Config::set_anchor_timeline(Timeline&)" << std::endl;
#endif
    m.lock();
    this->anchor = anchor;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- Config::set_anchor_timeline" << std::endl;
#endif
}

void Config::set_anchor_steps_per_second(time_t anchor_steps_per_second) {
#ifdef DEBUG
    std::cout << "-> Config::set_anchor_steps_per_second(" << anchor_steps_per_second << ")" << std::endl;
#endif
    m.lock();
    this->anchor_steps_per_second = anchor_steps_per_second;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- Config::set_anchor_steps_per_second" << std::endl;
#endif
}

void Config::update_anchor_steps_per_second() {
#ifdef DEBUG
    std::cout << "-> Config::update_anchor_steps_per_second()" << std::endl;
#endif
    time_t t0 = get_anchor_timeline().get_time();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    time_t tf = get_anchor_timeline().get_time();

    set_anchor_steps_per_second(tf - t0);
#ifdef DEBUG
    std::cout << "<- Config::update_anchor_steps_per_second" << std::endl;
#endif
}

LocalTimeline& Config::get_display_timeline() {
    m.lock();
    LocalTimeline& local = display_timeline;
    m.unlock();

    return local;
}

Config& Config::get_instance() {
    m_static.lock();
    if (instance == nullptr)
        instance = new Config(*new SystemTimeline());
    m_static.unlock();
    return *instance;
}

void Config::update_anchor_timeline(Timeline& anchor) {
#ifdef DEBUG
    std::cout << "-> Config::update_anchor_timeline(Timeline&)" << std::endl;
#endif
    bool running = get_running();
    if (running)
        throw std::invalid_argument("Cannot update anchor timeline during run");
    
    set_anchor_timeline(anchor);
    update_anchor_steps_per_second();
#ifdef DEBUG
    std::cout << "<- Config::update_anchor_timeline" << std::endl;
#endif
}

AbstractAssetFactoryConfig& Config::get_asset_factory_config() {
    m.lock();
    AbstractAssetFactoryConfig& local = asset_factory_config;
    m.unlock();

    return local;
}

time_t Config::get_anchor_steps_per_second() {
    m.lock();
    time_t local = anchor_steps_per_second;
    m.unlock();

    return local;
}
