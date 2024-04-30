#ifndef SPlat_Events_ControllerInterface_h
#define SPlat_Events_ControllerInterface_h

#include "Event.h"

namespace SPlat {

    class ControllerInterface {

    public:

        virtual void push_outgoing_event(Event) = 0;

        virtual Event pop_incoming_event() = 0;

        virtual bool has_incoming_event() = 0;

    };

}

#endif