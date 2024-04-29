#ifndef SPLAT_RUNTIME_H
#define SPLAT_RUNTIME_H

#include "AbstractAssetFactoryConfig.h"
#include "TimingConfigInterface.h"

#include <mutex>

namespace SPlat {

    class Config {

        static Config * instance;

        static std::mutex m_static;

        std::mutex m;

        bool running;

        TimingConfigInterface& timing_config;

        AbstractAssetFactoryConfig& asset_factory_config;

        Config();

    public:

        bool get_running();

        void set_running(bool);

        AbstractAssetFactoryConfig& get_asset_factory_config();

        TimingConfigInterface& get_timing_config();

        static Config& get_instance();

    };

}

#endif