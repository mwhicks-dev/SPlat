#ifndef SPlat_TimingConfigInterface_h
#define SPlat_TimingConfigInterface_h

#include "LocalTimeline.h"

namespace SPlat {

    class TimingConfigInterface {

    public:

        virtual Timeline& get_anchor_timeline() = 0;

        virtual LocalTimeline& get_display_timeline() = 0;

        virtual time_t get_anchor_steps_per_second() = 0;

    };

};

#endif