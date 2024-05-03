#ifndef SPlat_Server_h
#define SPlat_Server_h

#include "Entrypoint.h"

namespace SPlat {

    class Server : public Entrypoint {

        ConfigInterface& config;

        ControllerInterface& controller;

        Model::ObjectModelInterface& object_model;

        Events::ListenerInterface& background_listener;

        Server();

    public:

        ConfigInterface& get_config() override;

        Model::ObjectModelInterface& get_object_model() override;

        void start() override;

        static Server& get_instance();

        Events::ListenerInterface& get_background_listener() override;

        ControllerInterface& get_controller() override;

    };

}

#endif