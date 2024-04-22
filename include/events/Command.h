#ifndef SPLAT_EVENTS_COMMAND_H
#define SPLAT_EVENTS_COMMAND_H

#include <string>

namespace SPlat {

    namespace Events {

        /// @brief Command class as ran by listeners
        struct Command {

            /// @brief command priority; default to 0
            int priority = 0;

            /// @brief command type, derived from calling event
            std::string type;

            /// @brief args as passed from event serialized to string
            std::string args;

            template <class Archive>
            void serialize(Archive& ar) {
                ar(priority, type, args);
            }
            
            /// @brief overload < operator 
            bool operator<(const Command& other) {
                return priority < other.priority;
            }

            /// @brief overload > operator 
            bool operator>(const Command& other) {
                return priority > other.priority;
            }

            /// @brief overload == operator 
            bool operator==(const Command& other) {
                return priority == other.priority;
            }

            /// @brief overload != operator 
            bool operator!=(const Command& other) {
                return !(*this == other);
            }

            /// @brief overload <= operator 
            bool operator<=(const Command& other) {
                return !(*this > other);
            }

            /// @brief overload >= operator 
            bool operator>=(const Command& other) {
                return !(*this < other);
            }

        };

    };

}

#endif