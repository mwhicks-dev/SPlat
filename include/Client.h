#ifndef SPLAT_CLIENT_H
#define SPLAT_CLIENT_H

#include <queue>
#include <mutex>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "model/GameObjectModel.h"
#include "Controller.h"

namespace SPlat {

    class Client {

        /// @brief main display
        sf::RenderWindow window;

        /// @brief single controller for event handling
        Controller ctl;

        /// @brief performs keyboard press/release event checking
        /// @param key actual key to check
        void handle_key_event(sf::Keyboard::Key);

    public:

        void start();

    };

}

#endif