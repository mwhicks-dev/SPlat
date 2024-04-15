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
            /// @param position pos of new asset
            /// @param size size of new asset
            /// @param type new asset type -- should come from static asset ids
            CreateControlAssetEvent(sf::Vector2f position, sf::Vector2f size,
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