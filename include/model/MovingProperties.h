#ifndef SPLAT_MODEL_MOVINGPROPERTIES_H
#define SPLAT_MODEL_MOVINGPROPERTIES_H

#include "model/AssetProperties.h"

namespace SPlat {

    namespace Model {

        class MovingProperties : public AssetProperties {

        public:

            sf::Vector2f velocity;

            time_t last_updated;

            MovingProperties(size_t id, sf::Vector2f position, 
                    sf::Vector2f size, sf::Color fill_color, int
                    collision_priority, sf::Vector2f velocity, time_t
                    last_updated) : AssetProperties(id, position, size, 
                    fill_color, collision_priority) {
                set_velocity(velocity);
                set_last_update(last_updated);
            }

            MovingProperties(MovingProperties& other)
            : AssetProperties(other) {
                set_velocity(other.get_velocity());
                set_last_update(other.get_last_updated());
            }

            sf::Vector2f get_velocity() {
                m.lock();
                sf::Vector2f local = velocity;
                m.unlock();

                return local;
            }

            time_t get_last_updated() {
                m.lock();
                time_t local = last_updated;
                m.unlock();

                return local;
            }

            void set_velocity(sf::Vector2f velocity) {
                m.lock();
                this->velocity = velocity;
                m.unlock();
            }

            void set_last_update(time_t last_updated) {
                m.lock();
                this->last_updated = last_updated;
                m.unlock();
            }

        };

    }

}

#endif