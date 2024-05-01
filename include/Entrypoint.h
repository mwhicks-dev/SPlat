#ifndef SPlat_Entrypoint_h
#define SPlat_Entrypoint_h

#include "ConfigInterface.h"
#include "ControllerInterface.h"
#include "model/ObjectModelInterface.h"

#include <mutex>

namespace SPlat {

    class Entrypoint {

    protected:

        static std::mutex m;

        static Entrypoint * instance;

    public:

        virtual ConfigInterface& get_config() = 0;

        virtual Model::ObjectModelInterface& get_object_model() = 0;

        virtual void start() = 0;

        static Entrypoint& get_instance();

        virtual Events::ListenerInterface& get_background_listener() = 0;

        virtual ControllerInterface& get_controller() = 0;

    };

}

#endif