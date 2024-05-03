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
                const std::lock_guard<std::mutex> lock(m);
                return velocity;
            }

            time_t get_last_updated() {
                const std::lock_guard<std::mutex> lock(m);
                return last_updated;
            }

            void set_velocity(sf::Vector2f velocity) {
                const std::lock_guard<std::mutex> lock(m);
                this->velocity = velocity;
            }

            void set_last_update(time_t last_updated) {
                const std::lock_guard<std::mutex> lock(m);
                this->last_updated = last_updated;
            }

            template <class Archive>
            void serialize(Archive& ar) {
                ar(velocity, last_updated);
            }

        };

    }

}

#endif