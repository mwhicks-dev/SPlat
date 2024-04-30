#ifndef SPlat_TimingConfigInterface_h
#define SPlat_TimingConfigInterface_h

#include "LocalTimeline.h"

namespace SPlat {

    class TimingConfigInterface {

    public:

        virtual Timeline& get_anchor_timeline() = 0;

        virtual LocalTimeline& get_display_timeline() = 0;

        virtual long get_framerate_limit() = 0;

        virtual time_t get_anchor_steps_per_second() = 0;

        virtual void set_framerate_limit(long) = 0;

    };

};

#endif