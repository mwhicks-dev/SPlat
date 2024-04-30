#ifndef SPLAT_CLIENT_H
#define SPLAT_CLIENT_H

#include <queue>
#include <mutex>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "model/GameObjectModel.h"
#include "ConfigInterface.h"
#include "Controller.h"

namespace SPlat {

    class Client {

        std::mutex m;

        /// @brief main display
        sf::RenderWindow window;

        /// @brief single controller for event handling
        Controller ctl;

        ConfigInterface& config;

        /// @brief performs keyboard press/release event checking
        /// @param key actual key to check
        void handle_key_event(sf::Keyboard::Key);

        Client();

    public:

        void start();

        void update_framerate_limit(long); 

        ConfigInterface& get_config();

        static Client& get_instance();

    };

}

#endif