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

        /// @brief type -> handle(args)
        static std::map<std::string, void (*)(string)> handlers;

    public:

        /// @brief set type and args
        /// @param type event type id
        /// @param args specific event args
        Event(string, string);

        /// @brief runs handler with args
        /// @throws std::domain_error if type has not been specified 
        void dispatch();

        static void add_handler(std::string, void (*)(std::string));

    };

}

#endif