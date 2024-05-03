#ifndef SPLAT_MODEL_MOVINGPLATFORMPROPERTIES_H
#define SPLAT_MODEL_MOVINGPLATFORMPROPERTIES_H

#include "Serialization.h"

#include <mutex>

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

        class MovingPlatformProperties {

            std::mutex m;

            time_t last_state_change = -1;

            std::vector<State> states = {};

        public:

            MovingPlatformProperties() = default;

            MovingPlatformProperties(time_t last_state_change, std::vector<State> states) {
                set_last_state_change(last_state_change);
                set_states(states);
            }

            MovingPlatformProperties(MovingPlatformProperties& other) {
                set_last_state_change(other.get_last_state_change());
                set_states(other.get_states());
            }

            time_t get_last_state_change() {
                const std::lock_guard<std::mutex> lock(m);
                return last_state_change;
            }

            std::vector<State> get_states() {
                const std::lock_guard<std::mutex> lock(m);
                return states;
            }

            void set_states(std::vector<State> states) {
                const std::lock_guard<std::mutex> lock(m);
                this->states = states;
            }

            void set_last_state_change(time_t last_state_change) {
                const std::lock_guard<std::mutex> lock(m);
                this->last_state_change = last_state_change;
            }

        };

    }

}

#endif