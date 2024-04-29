#ifndef SPLAT_LOOPEVENT_H
#define SPLAT_LOOPEVENT_H

#include "events/Event.h"

#include <cereal/types/unordered_set.hpp>

namespace SPlat {

    namespace Events {

        /// @brief updates all assets on dispatch
        class TickEvent : public Event {

            /// @brief set of asset IDs to update
            std::unordered_set<size_t> ids;

        public:

            /// @brief TickEvent arguments
            struct Args {

                /// @brief set of asset IDs to update
                std::unordered_set<size_t> ids;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(ids);
                }

            };

            /// @brief creates TickEvent with current set of asset IDs
            TickEvent();

            static std::string get_type() { return "tick_event"; }

            void raise() override;

            static void handler(std::string);

        };

    }

}

#endif