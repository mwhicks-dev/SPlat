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
            /// @param position position of new asset
            /// @param size size of new asset
            /// @param type new asset type -- should come from asset static ids
            CreateAssetEvent(sf::Vector2f position, sf::Vector2f size, 
                    std::string type) {
                // Serialize args to JSON string
                SPlat::Model::AssetProperties args = {
                    .position=position, 
                    .size=size, 
                    .type=type
                };
                std::stringstream ss;
                {
                    cereal::JSONOutputArchive oar(ss);
                    oar(args);
                }

                this->type = TYPE;
                this->args = ss.str();

                // pass to background listener
                BackgroundListener &lst = BackgroundListener::get_instance();
                lst.push_event(*this);
            }

        };

    }

}

#endif