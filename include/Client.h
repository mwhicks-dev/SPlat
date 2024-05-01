#ifndef SPLAT_CLIENT_H
#define SPLAT_CLIENT_H

#include <queue>

#include <SFML/Window.hpp>

#include "model/ObjectModelInterface.h"
#include "events/ListenerInterface.h"
#include "Entrypoint.h"

namespace SPlat {

    class Client : public Entrypoint {

        /// @brief main display
        sf::RenderWindow window;

        ConfigInterface& config;

        Model::ObjectModelInterface& object_model;

        Events::ListenerInterface& foreground_listener;

        Events::ListenerInterface& background_listener;

        ControllerInterface& ctl;

        /// @brief performs keyboard press/release event checking
        /// @param key actual key to check
        void handle_key_event(sf::Keyboard::Key);

        Client();

    public:

        void start() override;

        ConfigInterface& get_config() override;

        Model::ObjectModelInterface& get_object_model() override;

        Events::ListenerInterface& get_foreground_listener();

        static Client& get_instance();

        Events::ListenerInterface& get_background_listener() override;

        ControllerInterface& get_controller() override;

    };

}

#endif