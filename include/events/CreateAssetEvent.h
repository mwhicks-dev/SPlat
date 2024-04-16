#ifndef SPLAT_CREATEASSETEVENT_H
#define SPLAT_CREATEASSETEVENT_H

#include <iostream>

#include "Serialization.h"
#include "events/Event.h"
#include "events/Listener.h"
#include "model/Asset.h"

#include "cereal/archives/json.hpp"

namespace SPlat {

    namespace Events {

        /// @brief creates asset on dispatch
        class CreateAssetEvent : public Event {

        public:

            /// @brief unique identifier for create asset event
            static std::string TYPE;

            /// @brief handler function that creates asset given args
            /// @param args serialized AssetProperties to use
            static void handler(std::string);

            /// @brief converts asset params to serializable form
            /// @param args properties of new Asset
            CreateAssetEvent(SPlat::Model::AssetProperties args) {
                // Serialize args to JSON string
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