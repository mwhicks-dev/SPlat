#ifndef SPlat_TimingConfig_h
#define SPlat_TimingConfig_h

#include "TimingConfigInterface.h"

namespace SPlat {

    class TimingConfig : public TimingConfigInterface {

        std::mutex m;

        Timeline& anchor_timeline;

        LocalTimeline& display_timeline;

        time_t anchor_steps_per_second;

        void set_anchor_steps_per_second(time_t);

    public:

        TimingConfig();

        Timeline& get_anchor_timeline() override;

        LocalTimeline& get_display_timeline() override;

        time_t get_anchor_steps_per_second() override;

    };

}

#endif