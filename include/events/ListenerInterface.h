#ifndef SPlat_Events_ListenerInterface_h
#define SPlat_Events_ListenerInterface_h

#include "events/handlers/EventHandlerInterface.h"
#include "Event.h"

namespace SPlat {

    namespace Events {

        class ListenerInterface {

        public:

            virtual void set_handler(std::string, EventHandlerInterface&) = 0;

            virtual void push_event(Event) = 0;

            virtual void run() = 0;

            virtual void await(Event) = 0;

        };

    }

}

#endif