#ifndef SPLAT_CREATECONTROLLASSETEVENT_H
#define SPLAT_CREATECONTROLLASSETEVENT_H

#include <iostream>

#include "events/CreateAssetEvent.h"
#include "events/Listener.h"
#include "Serialization.h"

#include "cereal/archives/json.hpp"

namespace SPlat {

    namespace Events {

        /// @brief hybrid create & control asset event
        class CreateControlAssetEvent : public Event {

        public:

            /// @brief unique identifier for create + control asset event
            static std::string TYPE;

            /// @brief handler function that creates + controls asset 
            /// @param args serialized AssetProperties to use
            static void handler(std::string);

            /// @brief converts asset params to serializable form
            /// @param args properties of new asset
            CreateControlAssetEvent(SPlat::Model::AssetProperties args) {
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