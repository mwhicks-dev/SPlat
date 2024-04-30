#ifndef SPLAT_CLIENT_H
#define SPLAT_CLIENT_H

#include <queue>

#include <SFML/Window.hpp>

#include "model/GameObjectModel.h"
#include "events/ListenerInterface.h"
#include "Entrypoint.h"

namespace SPlat {

    class Client : public Entrypoint {

        /// @brief main display
        sf::RenderWindow window;

        ConfigInterface& config;

        Events::ListenerInterface& foreground_listener;

        /// @brief performs keyboard press/release event checking
        /// @param key actual key to check
        void handle_key_event(sf::Keyboard::Key);

        Client();

    public:

        void start() override;

        ConfigInterface& get_config() override;

        Events::ListenerInterface& get_foreground_listener();

        static Client& get_instance();

    };

}

#endif