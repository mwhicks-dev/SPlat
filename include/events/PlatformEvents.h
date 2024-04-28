#ifndef SPLAT_EVENTS_PLATEVENTS_H
#define SPLAT_EVENTS_PLATEVENTS_H

#include "events/Event.h"
#include "model/Asset.h"

namespace SPlat {

    namespace Events {

        class CreatePlatformEvent : public Event {

            /// @brief iniital platform properties
            SPlat::Model::AssetProperties properties;

        public:

            struct Args {

                /// @brief initial platform properties
                SPlat::Model::AssetProperties properties;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(properties);
                }

            };

            /// @brief creates new CreatePlatformEvent from properties
            /// @param properties initial platform properties
            CreatePlatformEvent(SPlat::Model::AssetProperties);

            static std::string get_type() { return "create_platform_event"; }

            void raise() override;

            static void handler(std::string);

        };

    }

}

#endif