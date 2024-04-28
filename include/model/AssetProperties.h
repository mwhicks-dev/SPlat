#ifndef SPLAT_MODEL_ASSETPROPERTIES_H
#define SPLAT_MODEL_ASSETPROPERTIES_H

#include <SFML/Graphics.hpp>

#include <mutex>

namespace SPlat {

    namespace Model {

        class AssetProperties {

        public:

            AssetProperties(AssetProperties& other) {
                other.m.lock();
                m.lock();
                this->id = other.id;
                this->position = other.position;
                this->size = other.size;
                this->fill_color = other.fill_color;
                this->collision_priority = other.collision_priority;
                other.m.unlock();
                m.unlock();
            }

            std::mutex m;

            size_t id;

            sf::Vector2f position;

            sf::Vector2f size;

            sf::Color fill_color;

            int collision_priority;

        };

    }

}

#endif