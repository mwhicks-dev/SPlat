#include "Config.h"
#include "AssetFactoryConfig.h"
#include "Environment.h"
#include "TimingConfig.h"

#include <thread>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat;

Config::Config()
: asset_factory_config(*new AssetFactoryConfig()),
timing_config(*new TimingConfig()),
environment(*new Environment()) {
    get_timing_config().get_display_timeline()
        .set_tic(get_timing_config().get_anchor_steps_per_second());
    get_environment().set_running(true);
}

AssetFactoryConfigInterface& Config::get_asset_factory_config() {
    m.lock();
    AssetFactoryConfigInterface& local = asset_factory_config;
    m.unlock();

    return local;
}

EnvironmentInterface& Config::get_environment() {
    m.lock();
    EnvironmentInterface& local = environment;
    m.unlock();

    return local;
}

TimingConfigInterface& Config::get_timing_config() {
    m.lock();
    TimingConfigInterface& local = timing_config;
    m.unlock();

    return local;
}
