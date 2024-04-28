#ifndef SPLAT_MODEL_ASSET_H
#define SPLAT_MODEL_ASSET_H

#include <SFML/Graphics.hpp>

#include <mutex>

namespace SPlat {

    namespace Model {

        class AssetProperties {

        public:

            std::mutex m;

            size_t id;

            sf::Vector2f position;

            sf::Vector2f size;

            sf::Color fill_color;

            int collision_priority;

        };

        class MovingProperties : public AssetProperties {

        public:

            sf::Vector2f velocity;

            time_t last_updated;

        };

    }

}

#endif