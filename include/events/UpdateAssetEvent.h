#ifndef SPLAT_EVENT_UPDATEASSET_H
#define SPLAT_EVENT_UPDATEASSET_H

#include "cereal/archives/json.hpp"

#include "events/Event.h"
#include "events/Listener.h"
#include "model/Asset.h"
#include "Serialization.h"

namespace SPlat {

    namespace Events {

        /// @brief serializable container for ID and Asset properties
        struct UpdateAssetEventArgs {

            /// @brief ID of asset to update
            size_t id;

            /// @brief properties of asset to update
            SPlat::Model::AssetProperties properties;

            /// @brief serialization function for UpdateAssetEventArgs 
            template <class Archive>
            void serialize(Archive & ar) {
                ar(id, properties);
            }

        };

        /// @brief on dispatch updates asset by ID
        class UpdateAssetEvent : public Event {

        public:

            /// @brief unique identifier for update asset event
            static std::string TYPE;

            /// @brief handler function that updates asset given args
            /// @param args serialized UpdateAssetEventArgs to use
            static void handler(std::string);

            UpdateAssetEvent(size_t id, SPlat::Model::AssetProperties prop) {
                // Serialize args to JSON string
                UpdateAssetEventArgs args = {
                    .id=id,
                    .properties=prop
                };
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