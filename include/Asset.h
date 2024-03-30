#ifndef SPLAT_ASSET
#define SPLAT_ASSET

#include <SFML/Graphics.hpp>

namespace SPlat {

    class Asset : public sf::RectangleShape {

    public:

        size_t id;

        Asset * standing_on;

        sf::Vector2f velocity;

        Asset(sf::Vector2f&);

        virtual int get_priority() = 0;

    };

}

#endif