#ifndef SPLAT_LISTENER_H
#define SPLAT_LISTENER_H

#include <mutex>
#include <queue>
#include <map>

#include "events/Command.h"

namespace SPlat {

    namespace Events {

        /// @brief shared structure for listeners
        class Listener {

            /// @brief mutex to safeguard commands access
            std::mutex commands_lock;

            /// @brief queue of commands to dispatch
            std::priority_queue<Command> commands;

            /// @brief mutex to safeguard handlers access
            static std::mutex handlers_lock;

            /// @brief non-default handlers mapping
            static std::map<std::string, void (*)(std::string)> handlers;

        protected:

            /// @brief protected listener constructor
            Listener();

            /// @brief dispatch command using methods specified by their events
            /// @param cmd command to dispatch
            void dispatch(Command);

        public:

            /// @brief dequeue and dispatch commands on call
            void run();

            /// @brief externally enqueue a new command
            /// @param command command to enqueue
            void push_command(Command);

            /// @brief set handler by string command type
            /// @param type string command type
            /// @param handler handler function template
            void set_handler(std::string, void (*)(std::string));

        };

        /// @brief listener for hardware interface device commands
        class ForegroundListener : public Listener {

            /// @brief singleton constructor
            ForegroundListener() = default;

        public:

            /// @brief singleton getter
            /// @return instance of ForegroundListener
            static ForegroundListener& get_instance() {
                static ForegroundListener instance;
                return instance;
            }

        };

        /// @brief listener for general update/physics commands
        class BackgroundListener : public Listener {

            /// @brief singleton constructor
            BackgroundListener() = default;

        public:

            /// @brief singleton getter
            /// @return instance of BackgroundListener
            static BackgroundListener& get_instance() {
                static BackgroundListener instance;
                return instance;
            }

        };

    }

}

#endif