#ifndef SPLAT_MOVING_PLATFORM
#define SPLAT_MOVING_PLATFORM

#include "Platform.h"

namespace SPlat {

    namespace Model {

        class MovingPlatform : public Platform {

            size_t platform_ticks = 0;

        public:

            /// @brief MovingPlatform state; position, time and repeat
            struct State {

                /// @brief state position
                sf::Vector2f position;

                /// @brief number of ticks from position until next position
                size_t ticks_til_next;

                /// @brief flag to add this state back to queue (true default) 
                bool repeat = true;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(position, ticks_til_next, repeat);
                }

            };

        private:

            /// @brief "queue" of states to cycle between
            std::vector<State> queue;

        public:

            static std::string TYPE;

            MovingPlatform(sf::Vector2f&);

            int get_priority() override;

            std::string get_type() override;

            void add_state(State& s) { queue.push_back(s); }

            void update() override;

        };

    }

}

#endif