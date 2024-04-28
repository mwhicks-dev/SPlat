#ifndef SPLAT_EVENT_H
#define SPLAT_EVENT_H

#include <string>
#include <stdexcept>

namespace SPlat {

    namespace Events {

        /// @brief event interface; command invoker
        class Event {

            /// @brief creates Command and sends
            virtual void raise() = 0;

            /// @brief getter for event's string identifier
            static std::string get_type() 
                { throw std::invalid_argument("Not implemented"); }

            /// @brief handler method for event type
            static void handler(std::string) 
                { throw std::invalid_argument("Not implemented"); }

        };

    }

}

#endif