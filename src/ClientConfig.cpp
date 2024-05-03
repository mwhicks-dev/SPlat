#include "ClientConfig.h"
#include "AssetFactoryConfig.h"
#include "Environment.h"
#include "TimingConfig.h"

#include <thread>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat;

ClientConfig::ClientConfig()
: asset_factory_config(*new AssetFactoryConfig()),
timing_config(*new TimingConfig()),
environment(*new Environment()) {
    get_timing_config().get_display_timeline()
        .set_tic(get_timing_config().get_anchor_steps_per_second());
    get_environment().set_running(true);
}

AssetFactoryConfigInterface& ClientConfig::get_asset_factory_config() {
    const std::lock_guard<std::mutex> lock(m);
    return asset_factory_config;
}

EnvironmentInterface& ClientConfig::get_environment() {
    const std::lock_guard<std::mutex> lock(m);
    return environment;
}

TimingConfigInterface& ClientConfig::get_timing_config() {
    const std::lock_guard<std::mutex> lock(m);
    return timing_config;
}
