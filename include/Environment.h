#ifndef SPlat_Environment_h
#define SPlat_Environment_h

#include "EnvironmentInterface.h"

#include <mutex>

namespace SPlat {

    class Environment : public EnvironmentInterface {

        std::mutex m;

        long framerate_limit;

        bool running;

    public:

        Environment();

        long get_framerate_limit() override;

        bool get_running() override;

        void set_framerate_limit(long) override;

        void set_running(bool) override;

    };

}

#endif