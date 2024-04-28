#ifndef SPLAT_MODEL_MOVINGPLATFORMPROPERTIES_H
#define SPLAT_MODEL_MOVINGPLATFORMPROPERTIES_H

#include "model/MovingProperties.h"

namespace SPlat {

    namespace Model {

        class State {

        public:

            sf::Vector2f position;

            time_t anchor_steps;

            bool repeat;

        };

        class MovingPlatformProperties : public MovingProperties {

        public:

            std::vector<State> states;

            MovingPlatformProperties(MovingPlatformProperties& other)
            : MovingProperties(other) {
                set_states(other.get_states());
            }

            std::vector<State> get_states() {
                m.lock();
                std::vector<State> local = states;
                m.unlock();

                return local;
            }

            void set_states(std::vector<State> states) {
                m.lock();
                this->states = states;
                m.unlock();
            }

        };

    }

}

#endif