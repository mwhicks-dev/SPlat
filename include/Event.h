#ifndef SPLAT_EVENT_H
#define SPLAT_EVENT_H

#include "events/Command.h"

namespace SPlat {

    /// @brief event state
    struct Event {

        Events::Command command;

        size_t sender = 0;

        template <class Archive>
        void serialize(Archive& ar) {
            ar(command, sender);
        }
        
        /// @brief overload < operator 
        friend bool operator<(const Event& lhs, const Event& rhs) {
            return lhs.command.priority < rhs.command.priority;
        }

        /// @brief overload > operator 
        friend bool operator>(const Event& lhs, const Event& rhs) {
            return lhs.command.priority > rhs.command.priority;
        }

        /// @brief overload == operator 
        friend bool operator==(const Event& lhs, const Event& rhs) {
            return lhs.command.priority == rhs.command.priority;
        }

        /// @brief overload != operator 
        friend bool operator!=(const Event& lhs, const Event& rhs) {
            return !(lhs == rhs);
        }

        /// @brief overload <= operator 
        friend bool operator<=(const Event& lhs, const Event& rhs) {
            return !(lhs > rhs);
        }

        /// @brief overload >= operator 
        friend bool operator>=(const Event& lhs, const Event& rhs) {
            return !(lhs < rhs);
        }

    };

}

#endif