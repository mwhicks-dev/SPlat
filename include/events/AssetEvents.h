#ifndef SPLAT_ASSETEVENTS_H
#define SPLAT_ASSETEVENTS_H

#include "events/Event.h"
#include "Serialization.h"

#include <mutex>

namespace SPlat {

    namespace Events {

        /// @brief superclass of events used to add/set vector values
        class VectorEvent : public Event {

        public:

            /// @brief shared arguments of events used to add/set vector values
            struct Args {

                /// @brief id of asset to update
                size_t id;

                /// @brief value to update with
                sf::Vector2f modifier;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(id, modifier);
                }

            };

        };

        /// @brief event used to add velocity to asset
        class AddVelocityEvent : public VectorEvent {

            /// @brief ID of asset to add velocity to
            size_t id;

            /// @brief velocity to add
            sf::Vector2f velocity;

        public:

            /// @brief create new AddVelocityEvent given id and velocity
            /// @param id ID of asset to update
            /// @param velocity velocity to add to asset matching ID
            AddVelocityEvent(size_t, sf::Vector2f);

            void raise() override;

            static std::string get_type() { return "add_velocity_event"; }

            static void handler(std::string);

        };

        /// @brief event used to add position to asset
        class AddPositionEvent : public VectorEvent {

            /// @brief ID of asset to add velocity to
            size_t id;

            /// @brief velocity to add
            sf::Vector2f position;

        public:

            /// @brief create new AddVelocityEvent given id and velocity
            /// @param id ID of asset to update
            /// @param position velocity to add to asset matching ID
            AddPositionEvent(size_t, sf::Vector2f);

            void raise() override;

            static std::string get_type() { return "add_position_event"; }

            static void handler(std::string);

        };

        /// @brief event used to set controlled asset
        class ControlAssetEvent : public Event {

            static std::mutex control_lock;
            
            static bool control_set;

            static size_t control;

            /// @brief ID of asset to control
            size_t id;

        public:

            /// @brief ControlAssetEvent arguments
            struct Args {

                /// @brief ID of asset to control
                size_t id;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(id);
                }

            };

            /// @brief create a new ControlAssetEvent with an asset ID
            /// @param id ID of asset to update
            ControlAssetEvent(size_t);
            
            void raise() override;

            static std::string get_type() { return "control_asset_event"; }

            static void handler(std::string);

            static size_t get_controlled_asset_id();

        };

    }

}

#endif