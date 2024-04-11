#ifndef SPLAT_CREATECONTROLLASSETEVENT_H
#define SPLAT_CREATECONTROLLASSETEVENT_H

#include <iostream>

#include "events/CreateAssetEvent.h"
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
            /// @param args serialized CreateAssetEventArgs to use
            static void handler(std::string);

            /// @brief converts asset params to serializable form
            /// @param position pos of new asset
            /// @param size size of new asset
            /// @param type new asset type -- should come from static asset ids
            CreateControlAssetEvent(sf::Vector2f position, sf::Vector2f size,
                    std::string type) {
                // Serialize args to JSON string
                CreateAssetEventArgs args = {position, size, type};
                std::stringstream ss;
                {
                    cereal::JSONOutputArchive oar(ss);
                    oar(args);
                }

                this->type = TYPE;
                this->args = ss.str();
            }

        }

    }

}

#endif