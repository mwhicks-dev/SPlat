#ifndef SPLAT_LOOPEVENT_H
#define SPLAT_LOOPEVENT_H

#include "events/Event.h"
#include "events/Listener.h"
#include "model/GameObjectModel.h"
#include "Serialization.h"

#include "cereal/cereal.hpp"
#include "cereal/types/unordered_set.hpp"
#include "cereal/archives/json.hpp"

#include <string>
#include <iostream>

namespace SPlat {

    namespace Events {

        /// @brief struct containing all asset IDs at tick
        struct TickEventArgs {

            /// @brief set of IDs to update
            std::unordered_set<size_t> ids;

            /// @brief formulation for converting args to string
            /// @param ar archive class used in serialization
            template <class Archive>
            void serialize(Archive &ar) {
                ar(ids);
            }

        };

        /// @brief updates all assets on dispatch
        class TickEvent : public Event {

        public:

            /// @brief unique identifier for tick event
            static std::string TYPE;

            /// @brief handler function that ticks all assets
            /// @param args serialized TickEventArgs
            static void handler(std::string);

            /// @brief creates TickEvent with current set of asset IDs
            TickEvent() {
                // serialize args to JSON
                TickEventArgs args = {
                    SPlat::Model::GameObjectModel::get_instance()
                        .getIds()
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