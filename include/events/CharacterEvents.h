#ifndef SPLAT_EVENTS_CHARACTEREVENTS_H
#define SPLAT_EVENTS_CHARACTEREVENTS_H

#include "events/Event.h"
#include "model/CharacterProperties.h"
#include "model/MovingProperties.h"

#include <cereal/archives/json.hpp>

namespace SPlat {

    namespace Events {

        /// @brief creates a character from properties
        class CreateCharacterEvent : public Event {

        protected:

            /// @brief properties to create from
            SPlat::Model::MovingProperties properties;

        public:

            /// @brief create character args
            struct Args {

                /// @brief AssetProperties to create character from
                SPlat::Model::MovingProperties properties;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(properties);
                }

            };

            /// @brief create new CreateCharacterEvent from properties
            /// @param properties AssetProperties to create asset from
            CreateCharacterEvent(SPlat::Model::MovingProperties);

            void raise() override;

            static std::string get_type() { return "create_character_event"; }

            static void handler(std::string);

        };

        /// @brief create asset, and set it as controlled as well
        class CreateControlCharacterEvent : public CreateCharacterEvent {

        public:

            /// @brief create new CreateControlCharacterEvent from properties
            /// @param properties AssetProperties to create from
            CreateControlCharacterEvent(SPlat::Model::MovingProperties properties)
            : CreateCharacterEvent(properties) {}

            void raise() override;

            static std::string get_type() { return "create_control_character_event"; }

            static void handler(std::string);

        };

    }

}

#endif