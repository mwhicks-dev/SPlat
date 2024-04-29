#ifndef SPlat_Environment_h
#define SPlat_Environment_h

#include "EnvironmentInterface.h"

#include <mutex>

namespace SPlat {

    class Environment : public EnvironmentInterface {

        std::mutex m;

        bool running;

    public:

        Environment();

        bool get_running() override;

        void set_running(bool) override;

    };

}

#endif