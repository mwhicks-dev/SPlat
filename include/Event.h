#ifndef SPLAT_EVENT_H
#define SPLAT_EVENT_H

#include <SFML/Graphics.hpp>

namespace SPlat {

    /** Event interface */
    class Event {

        /// @brief event type identifier
        std::string type;

        /// @brief event args
        std::string args;

    public:

        /// @brief type -> handle(args)
        static std::map<std::string, void (*)(string)> handlers;

        /// @brief set type and args
        /// @param type event type id
        /// @param args specific event args
        Event(string type, string args) {
            this.type = type;
            this.args = args;
        }

        /// @brief runs handler with args
        /// @throws std::domain_error if type has not been specified 
        void dispatch() {
            // throw domain error if no event spec
            if (handlers.count(type) == 0)
                throw std::domain_error("No such event type " + type);
            
            handler[type](args);
        }

        static void set_handler(std::string type, void (*func)(std::string)) {
            // override existing/defaults OK
            handlers[type] = func;
        }

    };

}

#endif