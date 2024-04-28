#ifndef SPLAT_EVENTS_MPLATEVENTS_H
#define SPLAT_EVENTS_MPLATEVENTS_H

#include "events/Event.h"
#include "model/MovingPlatform.h"

#include <cereal/types/vector.hpp>

namespace SPlat {

    namespace Events {

        /// @brief create moving platform from properties and states
        class CreateMovingPlatformEvent : public Event {

            /// @brief initial moving platform properties
            SPlat::Model::AssetProperties properties;

            /// @brief initial moving platform states list
            std::vector<SPlat::Model::MovingPlatform::State> states;

        public:

            /// @brief event arguments; asset properties and states list
            struct Args {

                /// @brief initial moving platform properties
                SPlat::Model::AssetProperties properties;

                /// @brief initial moving platform states list
                std::vector<SPlat::Model::MovingPlatform::State> states;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(properties, states);
                }

            };

            /// @brief creates new CreateMovingPlatformEvent from properties 
            ///     and states
            /// @param properties initial moving platform AssetProperties
            /// @param states initial moving platform states
            CreateMovingPlatformEvent(SPlat::Model::AssetProperties,
                    std::vector<SPlat::Model::MovingPlatform::State>);
            
            static std::string get_type() { return "create_moving_platform_event"; }

            void raise() override;

            static void handler(std::string);

        };

    }

}

#endif