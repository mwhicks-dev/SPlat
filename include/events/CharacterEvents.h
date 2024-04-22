#ifndef SPLAT_EVENTS_CHARACTEREVENTS_H
#define SPLAT_EVENTS_CHARACTEREVENTS_H

#include "events/Event.h"
#include "model/Asset.h"

#include <cereal/archives/json.hpp>

namespace SPlat {

    namespace Events {

        /// @brief creates a character from properties
        class CreateCharacterEvent : public Event {

        public:

            struct Args {

                SPlat::Model::AssetProperties properties;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(properties);
                }

            };

            /// @brief unique identifier for create asset event
            static std::string TYPE;

            /// @brief given properties, create new character
            /// @param  
            static void handler(std::string);

            CreateCharacterEvent(SPlat::Model::AssetProperties properties) {
                // serialize properties to JSON
                Args args = {.properties=properties};
                std::stringstream ss;
                {
                    cereal::JSONOutputArchive oar(ss);
                    oar(args);
                }

                this->type = TYPE;
                this->args = ss.str();
                this->foreground = false;
            }

        };

        class CreateControlCharacterEvent : public Event {

        public:

            struct Args {

                SPlat::Model::AssetProperties properties;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(properties);
                }

            };

            /// @brief unique identifier for create asset event
            static std::string TYPE;

            /// @brief given properties, create new character
            /// @param  
            static void handler(std::string);

            CreateControlCharacterEvent(SPlat::Model::AssetProperties properties) {
                // serialize properties to JSON
                Args args = {.properties=properties};
                std::stringstream ss;
                {
                    cereal::JSONOutputArchive oar(ss);
                    oar(args);
                }

                this->type = TYPE;
                this->args = ss.str();
                this->foreground = false;
            }

        };

    }

}

#endif