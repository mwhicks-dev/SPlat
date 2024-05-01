#ifndef SPLAT_EVENTS_COMMAND_H
#define SPLAT_EVENTS_COMMAND_H

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        /// @brief Command class as ran by listeners
        struct Command {

            /// @brief command priority; default to 0
            int priority = 0;

            /// @brief command type, derived from calling event
            std::string type = "";

            /// @brief args as passed from event serialized to string
            std::string args = "";

            template <class Archive>
            void serialize(Archive& ar) {
                ar(priority, type, args);
            }
            
            /// @brief overload < operator 
            friend bool operator<(const Command& lhs, const Command& rhs) {
                return lhs.priority < rhs.priority;
            }

            /// @brief overload > operator 
            friend bool operator>(const Command& lhs, const Command& rhs) {
                return lhs.priority > rhs.priority;
            }

            /// @brief overload == operator 
            friend bool operator==(const Command& lhs, const Command& rhs) {
                return lhs.priority == rhs.priority;
            }

            /// @brief overload != operator 
            friend bool operator!=(const Command& lhs, const Command& rhs) {
                return !(lhs == rhs);
            }

            /// @brief overload <= operator 
            friend bool operator<=(const Command& lhs, const Command& rhs) {
                return !(lhs > rhs);
            }

            /// @brief overload >= operator 
            friend bool operator>=(const Command& lhs, const Command& rhs) {
                return !(lhs < rhs);
            }

        };

    };

}

#endif