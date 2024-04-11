#ifndef SPLAT_CONTROLLER_H
#define SPLAT_CONTROLLER_H

#include <mutex>
#include <queue>

#include "events/Event.h"

namespace SPlat {

    /// @brief class to handle events and physics
    class Controller {

        /// @brief mutex needed to access events
        static std::mutex events_lock;

        /// @brief plain-old queue of events (pq later)
        static std::queue<Events::Event> events;

    public:

        /// @brief simple constructor for controller class
        Controller() = default;

        /// @brief continually dequeues and handles events
        /// @param runtime shared flag and mutex to check if client is running
        void run(std::pair<bool, std::mutex>&);

        /// @brief push a new event to the event queue
        /// @param event new event to push
        static void push_event(Events::Event);

    };

}

#endif