#ifndef SPlat_Events_ControllerInterface_h
#define SPlat_Events_ControllerInterface_h

#include "Request.h"
#include "Response.h"

namespace SPlat {

    class ControllerInterface {

    public:

        virtual void push_outgoing_request(Request) = 0;

        virtual Response pop_incoming_response() = 0;

        virtual bool has_incoming_response() = 0;

    };

}

#endif