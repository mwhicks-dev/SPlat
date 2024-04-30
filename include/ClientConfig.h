#ifndef SPLAT_RUNTIME_H
#define SPLAT_RUNTIME_H

#include "ConfigInterface.h"

#include <mutex>

namespace SPlat {

    class ClientConfig : public ConfigInterface {

        std::mutex m;

        AssetFactoryConfigInterface& asset_factory_config;

        EnvironmentInterface& environment;

        TimingConfigInterface& timing_config;

    public:

        AssetFactoryConfigInterface& get_asset_factory_config() override;

        EnvironmentInterface& get_environment() override;

        TimingConfigInterface& get_timing_config() override;

        ClientConfig();

    };

}

#endif