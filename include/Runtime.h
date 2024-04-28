#ifndef SPLAT_RUNTIME_H
#define SPLAT_RUNTIME_H

#include "LocalTimeline.h"
#include "model/AbstractAssetFactory.h"

#include <mutex>

namespace SPlat {

    class Runtime {

        static Runtime * instance;

        static std::mutex m_static;

        std::mutex m;

        bool running;

        Timeline& anchor;

        LocalTimeline display_timeline;

        time_t anchor_steps_per_second;

        Model::AbstractAssetFactory& character_factory;

        Model::AbstractAssetFactory& platform_factory;

        Model::AbstractAssetFactory& moving_platform_factory;

        Runtime(Timeline&);

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

        Model::AbstractAssetFactory& get_character_factory();

        Model::AbstractAssetFactory& get_platform_factory();

        Model::AbstractAssetFactory& get_moving_platform_factory();

        static Runtime& get_instance();

    };

}

#endif