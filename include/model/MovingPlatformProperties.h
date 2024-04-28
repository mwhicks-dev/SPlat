#ifndef SPLAT_MODEL_MOVINGPLATFORMPROPERTIES_H
#define SPLAT_MODEL_MOVINGPLATFORMPROPERTIES_H

#include "Serialization.h"
#include "model/MovingProperties.h"

namespace SPlat {

    namespace Model {

        class State {

        public:

            sf::Vector2f velocity;

            time_t anchor_steps;

            bool repeat;

            State() = default;

            State(sf::Vector2f velocity, time_t anchor_steps, bool repeat) {
                this->velocity = velocity;
                this->anchor_steps = anchor_steps;
                this->repeat = repeat;
            }

            template <class Archive>
            void serialize(Archive& ar) {
                ar(velocity, anchor_steps, repeat);
            }

        };

        class MovingPlatformProperties : public MovingProperties {

        public:

            time_t last_state_change;

            std::vector<State> states;

            MovingPlatformProperties() = default;

            MovingPlatformProperties(sf::Vector2f position, sf::Vector2f size, 
                    sf::Color fill_color, sf::Vector2f velocity, time_t 
                    last_updated, time_t last_state_change, std::vector<State> 
                    states) : MovingProperties(position, size, fill_color, -1, 
                    velocity, last_updated) {
                set_last_state_change(last_state_change);
                set_states(states);
            }

            MovingPlatformProperties(MovingPlatformProperties& other)
            : MovingProperties(other) {
                set_states(other.get_states());
            }

            time_t get_last_state_change() {
                m.lock();
                time_t local = last_state_change;
                m.unlock();

                return local;
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

            void set_last_state_change(time_t last_state_change) {
                m.lock();
                this->last_state_change = last_state_change;
                m.unlock();
            }

        };

    }

}

#endif