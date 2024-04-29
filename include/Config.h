#ifndef SPLAT_RUNTIME_H
#define SPLAT_RUNTIME_H

#include "AssetFactoryConfigInterface.h"
#include "EnvironmentInterface.h"
#include "TimingConfigInterface.h"

#include <mutex>

namespace SPlat {

    class Config {

        std::mutex m;

        AssetFactoryConfigInterface& asset_factory_config;

        EnvironmentInterface& environment;

        TimingConfigInterface& timing_config;

        Config();

    public:

        AssetFactoryConfigInterface& get_asset_factory_config();

        EnvironmentInterface& get_environment();

        TimingConfigInterface& get_timing_config();

        static Config& get_instance();

    };

}

#endif