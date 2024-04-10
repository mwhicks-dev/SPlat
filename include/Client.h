#ifndef SPLAT_CLIENT_H
#define SPLAT_CLIENT_H

#include <queue>
#include <mutex>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "model/GameObjectModel.h"

namespace SPlat {

    class Client {

        sf::RenderWindow window;

    public:

        void start();

    };

}

#endif