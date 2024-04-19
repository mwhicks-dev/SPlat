#ifndef SPLAT_ASSETEVENTS_H
#define SPLAT_ASSETEVENTS_H

#include "events/Event.h"
#include "model/Asset.h"
#include "Serialization.h"

#include "cereal/archives/json.hpp"

#include <iostream>

namespace SPlat {

    namespace Events {

        class VectorEvent : public Event {

        public:

            struct Args {

                size_t id;

                sf::Vector2f modifier;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(id, modifier);
                }

            };

        };

        /// @brief event used to add velocity to asset
        class AddVelocityEvent : public VectorEvent {

        public:

            /// @brief unique string identifier of asset
            static std::string TYPE;

            /// @brief given asset, update velocity
            /// @param serialized JSONified Args
            static void handler(std::string);

            /// @brief create new AddVelocityEvent given id and velocity
            /// @param id ID of asset to update
            /// @param velocity velocity to add to asset matching ID
            AddVelocityEvent(size_t id, sf::Vector2f velocity) {
                // Serialize args to JSON string
                Args args = { .id=id, .modifier=velocity };
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

        class AddPositionEvent : public VectorEvent {

        public:

            static std::string TYPE;

            static void handler(std::string);

            AddPositionEvent(size_t id, sf::Vector2f position) {
                // Serialize args to JSON string
                Args args = {.id=id, .modifier=position};
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