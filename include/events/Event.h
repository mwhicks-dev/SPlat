#ifndef SPLAT_EVENT_H
#define SPLAT_EVENT_H

#include <map>
#include <string>
#include <mutex>

#include <SFML/Graphics.hpp>

namespace SPlat {

    namespace Events {

        /** Event interface */
        class Event {

        public:

            /// @brief priority of event (default zero) for priority queueing
            int priority = 0;

            /// @brief event type identifier
            std::string type;

            /// @brief event args
            std::string args;

            /// @brief flag indicating whether or not event is fore/background
            bool foreground;

            /// @brief safeguard for handlers
            static std::mutex handlers_lock;

            /// @brief type -> handle(args)
            static std::map<std::string, void (*)(std::string)> handlers;

            /// @brief set type and args
            /// @param type event type id
            /// @param args specific event args
            /// @param foreground true if foreround event, false o/w (default)
            Event(std::string type, std::string args, bool foreground) {
                this->type = type;
                this->args = args;
                this->foreground = foreground;
            }

            /// @brief specific cases require setting type/args later
            Event() { this->foreground = false; };

            /// @brief runs handler with args
            /// @throws std::domain_error if type has not been specified 
            void dispatch() {
                // throw domain error if no event spec
                handlers_lock.lock();
                if (handlers.count(type) == 0) {
                    handlers_lock.unlock();
                    throw std::domain_error("No such event type " + type);
                }
                void (*handler)(std::string) = handlers[type];
                handlers_lock.unlock();
                
                handler(args);
            }

            /// @brief sends event to listener according to foreground flag
            void raise();

            friend bool operator==(const Event& lhs, const Event& rhs) {
                return lhs.priority == rhs.priority;
            }

            friend bool operator!=(const Event& lhs, const Event& rhs) {
                return !(lhs == rhs);
            }

            friend bool operator<=(const Event& lhs, const Event& rhs) {
                return !(lhs > rhs);
            }

            friend bool operator>=(const Event& lhs, const Event& rhs) {
                return !(lhs < rhs);
            }

            friend bool operator<(const Event& lhs, const Event& rhs) {
                return lhs.priority < rhs.priority;
            }

            friend bool operator>(const Event& lhs, const Event& rhs) {
                return lhs.priority > rhs.priority;
            }

        };

    }

}

#endif