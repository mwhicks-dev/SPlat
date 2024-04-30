#ifndef SPlat_ConfigInterface_h
#define SPlat_ConfigInterface_h

#include "AssetFactoryConfigInterface.h"
#include "EnvironmentInterface.h"
#include "TimingConfigInterface.h"

namespace SPlat {

    class ConfigInterface {

    public:

        virtual AssetFactoryConfigInterface& get_asset_factory_config() = 0;

        virtual EnvironmentInterface& get_environment() = 0;

        virtual TimingConfigInterface& get_timing_config() = 0;

    };

};

#endif