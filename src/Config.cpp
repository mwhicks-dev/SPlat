#include "Config.h"
#include "AssetFactoryConfig.h"
#include "TimingConfig.h"

#include <thread>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat;

Config::Config()
: asset_factory_config(*new AssetFactoryConfig()),
timing_config(*new TimingConfig()) {
    get_timing_config().get_display_timeline()
        .set_tic(get_timing_config().get_anchor_steps_per_second());
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

Config& Config::get_instance() {
    static Config instance;
    return instance;
}

AssetFactoryConfigInterface& Config::get_asset_factory_config() {
    m.lock();
    AssetFactoryConfigInterface& local = asset_factory_config;
    m.unlock();

    return local;
}

TimingConfigInterface& Config::get_timing_config() {
    m.lock();
    TimingConfigInterface& local = timing_config;
    m.unlock();

    return local;
}
