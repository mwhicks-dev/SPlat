#ifndef SPLAT_CONTROLASSETEVENT_H
#define SPLAT_CONTROLASSETEVENT_H

#include <mutex>

#include <cereal/archives/json.hpp>

#include "events/Event.h"
#include "Serialization.h"

namespace SPlat {

    namespace Events {

        /// @brief struct with asset ID to set control over
        struct ControlAssetEventArgs {
            
            /// @brief ID of asset to control
            size_t id;

            /// @brief formulation for converting args to string
            /// @param ar archive class used in serialization
            template <class Archive>
            void serialize(Archive &ar) {
                ar(id);
            }

        };

        /// @brief sets asset to be controlled on dispatch
        class ControlAssetEvent : public Event {

        public:

            /// @brief ID of asset controlled
            static size_t controlled;

            /// @brief checks if controlled asset has been defined;
            static bool controlled_set;

            /// @brief lock to guard access to controlled ID
            static std::mutex controlled_lock;

            /// @brief unique identifier for control asset event
            static std::string TYPE;

            /// @brief handler function that sets controlled asset given args
            /// @param  
            static void handler(std::string);

            /// @brief converts asset ID to serializable form
            /// @param id ID of asset to take control of
            ControlAssetEvent(size_t id) {
                // Serialize args to JSON string
                ControlAssetEventArgs args = {id};
                std::stringstream ss;
                {
                    cereal::JSONOutputArchive oar(ss);
                    oar(args);
                }

                this->type = TYPE;
                this->args = ss.str();
            }

            /// @brief gets ID of controlled asset
            /// @return ID of controlled asset
            /// @throws logic_error if no controlled asset
            static size_t get_controlled_asset_id() {
                controlled_lock.lock();
                if (!controlled_set) {
                    controlled_lock.unlock();
                    throw std::logic_error("No asset set as controlled");
                }
                size_t out = controlled;
                controlled_lock.unlock();

                return out;
            }

        };

    }

}

#endif