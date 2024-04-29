#ifndef SPLAT_RUNTIME_H
#define SPLAT_RUNTIME_H

#include "LocalTimeline.h"
#include "AbstractAssetFactoryConfig.h"

#include <mutex>

namespace SPlat {

    class Config {

        static Config * instance;

        static std::mutex m_static;

        std::mutex m;

        bool running;

        Timeline& anchor;

        LocalTimeline display_timeline;

        time_t anchor_steps_per_second;

        AbstractAssetFactoryConfig& asset_factory_config;

        Config(Timeline&);

        void set_anchor_timeline(Timeline&);

        void set_anchor_steps_per_second(time_t);

        void update_anchor_steps_per_second();

    public:

        bool get_running();

        void set_running(bool);

        Timeline& get_anchor_timeline();

        void update_anchor_timeline(Timeline&);

        LocalTimeline& get_display_timeline();

        time_t get_anchor_steps_per_second();

        AbstractAssetFactoryConfig& get_asset_factory_config();

        static Config& get_instance();

    };

}

#endif