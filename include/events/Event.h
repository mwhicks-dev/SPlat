#ifndef SPLAT_EVENT_H
#define SPLAT_EVENT_H

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

namespace SPlat {

    namespace Events {

        /** Event interface */
        class Event {

        public:

            /// @brief event type identifier
            std::string type;

            /// @brief event args
            std::string args;

            /// @brief type -> handle(args)
            static std::map<std::string, void (*)(std::string)> handlers;

            /// @brief set type and args
            /// @param type event type id
            /// @param args specific event args
            Event(std::string type, std::string args) {
                this->type = type;
                this->args = args;
            }

            /// @brief specific cases require setting type/args later
            Event() = default;

            /// @brief runs handler with args
            /// @throws std::domain_error if type has not been specified 
            void dispatch() {
                // throw domain error if no event spec
                if (handlers.count(type) == 0)
                    throw std::domain_error("No such event type " + type);
                
                handlers[type](args);
            }

        };

    }

}

#endif