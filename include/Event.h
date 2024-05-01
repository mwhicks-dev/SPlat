#ifndef SPLAT_EVENT_H
#define SPLAT_EVENT_H

#include "events/Command.h"

namespace SPlat {

    /// @brief event state
    struct Event {

        time_t event_time = 0;

        Events::Command command;

        bool client_side = true;

        size_t sender = 0;

        template <class Archive>
        void serialize(Archive& ar) {
            ar(event_time, command, client_side, sender);
        }
        
        /// @brief overload < operator 
        friend bool operator<(const Event& lhs, const Event& rhs) {
            return lhs.event_time < rhs.event_time;
        }

        /// @brief overload > operator 
        friend bool operator>(const Event& lhs, const Event& rhs) {
            return lhs.event_time > rhs.event_time;
        }

        /// @brief overload == operator 
        friend bool operator==(const Event& lhs, const Event& rhs) {
            return lhs.event_time == rhs.event_time;
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