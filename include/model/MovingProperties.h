#ifndef SPLAT_MODEL_MOVINGPROPERTIES_H
#define SPLAT_MODEL_MOVINGPROPERTIES_H

#include "Serialization.h"

#include <mutex>

namespace SPlat {

    namespace Model {

        class MovingProperties {

            std::mutex m;

        public:

            sf::Vector2f velocity = sf::Vector2f(0, 0);

            time_t last_updated = 0;

            MovingProperties() = default;

            MovingProperties(sf::Vector2f velocity, time_t last_updated) {
                set_velocity(velocity);
                set_last_update(last_updated);
            }

            MovingProperties(MovingProperties& other) {
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

            template <class Archive>
            void serialize(Archive& ar) {
                ar(velocity, last_updated);
            }

        };

    }

}

#endif