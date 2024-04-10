#ifndef SPLAT_CREATEASSETEVENT_H
#define SPLAT_CREATEASSETEVENT_H

#include <iostream>

#include "events/Event.h"

#include "Serialization.h"

#include "cereal/archives/json.hpp"

namespace SPlat {

    namespace Events {

        /// @brief struct containing position/size/type of asset to create
        struct CreateAssetEventArgs {

            /// @brief position of created asset
            sf::Vector2f position;

            /// @brief size of created asset
            sf::Vector2f size;

            /// @brief type of created asset; should come directly from concrete asset
            std::string type;

            /// @brief formulation for converting args to string
            /// @param ar archive class used in serialization
            template <class Archive>
            void serialize(Archive &ar) {
                ar(position, size, type);
            }

        };

        /// @brief creates asset on dispatch
        class CreateAssetEvent : public Event {

        public:

            /// @brief unique identifier for create asset event
            static std::string CREATE_ASSET_EVENT_TAG;

            /// @brief handler function that creates asset given args
            /// @param args serialized CreateAssetEventArgs to use
            static void handler(std::string);

            /// @brief converts asset params to serializable form
            /// @param position position of new asset
            /// @param size size of new asset
            /// @param type new asset type -- should come from asset static ids
            CreateAssetEvent(sf::Vector2f position, sf::Vector2f size, 
                    std::string type) {
                // Serialize args to JSON string
                CreateAssetEventArgs args = {position, size, type};
                std::stringstream ss;
                {
                    cereal::JSONOutputArchive oar(ss);
                    oar(args);
                }

                this->type = CREATE_ASSET_EVENT_TAG;
                this->args = ss.str();
            }

        };

    }

}

#endif