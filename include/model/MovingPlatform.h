#ifndef SPLAT_MOVING_PLATFORM
#define SPLAT_MOVING_PLATFORM

#include "Platform.h"

namespace SPlat {

    namespace Model {

        class MovingPlatform : public Platform {

            /// @brief "queue" of states to cycle between
            std::vector<SPlat::Model::MovingPlatform::State> queue;

        public:

            /// @brief MovingPlatform state; position, time and repeat
            class State {

                /// @brief state position
                sf::Vector2f position;

                /// @brief number of ticks from position until next position
                size_t ticks_til_next;

                /// @brief flag to add this state back to queue (true default) 
                bool repeat = true;

            };

            static std::string TYPE;

            MovingPlatform(sf::Vector2f&);

            int get_priority() override;

            std::string get_type() override;

            void add_state(State&);

            void update() override;

        };

    }

}

#endif