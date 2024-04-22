#ifndef SPLAT_EVENTS_MPLATEVENTS_H
#define SPLAT_EVENTS_MPLATEVENTS_H

#include "events/Event.h"
#include "model/MovingPlatform.h"

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

namespace SPlat {

    namespace Events {

        class CreateMovingPlatformEvent : public Event {

        public:

            /// @brief event arguments; asset properties and states list
            struct Args {

                SPlat::Model::AssetProperties properties;

                std::vector<SPlat::Model::MovingPlatform::State> states;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(properties, states);
                }

            };

            static std::string TYPE;

            static void handler(std::string);

            CreateMovingPlatformEvent(SPlat::Model::AssetProperties properties,
                    std::vector<SPlat::Model::MovingPlatform::State> states) {
                // serialize properties to JSON
                Args args = {.properties=properties, .states=states};
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