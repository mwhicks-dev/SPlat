#ifndef SPLAT_LISTENER_H
#define SPLAT_LISTENER_H

#include <mutex>
#include <queue>

#include "events/Event.h"

namespace SPlat {

    namespace Events {

        /// @brief shared structure for listeners
        class Listener {

            /// @brief mutex to safeguard events access
            std::mutex events_lock;

            /// @brief queue of events to dispatch
            std::priority_queue<Event> events;

        protected:

            /// @brief protected listener constructor
            Listener() = default;

        public:

            /// @brief dequeue and dispatch events on call
            void run();

            /// @brief externally enqueue a new event
            /// @param event event to enqueue
            void push_event(Event);

        };

        /// @brief listener for hardware interface device events
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

        /// @brief listener for general update/physics events
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