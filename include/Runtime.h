#ifndef SPLAT_RUNTIME_H
#define SPLAT_RUNTIME_H

#include "LocalTimeline.h"

#include <mutex>

namespace SPlat {

    class Runtime {

        static Runtime * instance;

        static std::mutex m_static;

        std::mutex m;

        bool running;

        Timeline& anchor;

        LocalTimeline display_timeline;

        Runtime(Timeline&);

    public:

        bool get_running();

        void set_running(bool);

        Timeline& get_anchor_timeline();

        void set_anchor_timeline(Timeline&);

        LocalTimeline& get_display_timeline();

        static Runtime& get_instance();

    };

}

#endif