#ifndef SPlat_Events_ControllerInterface_h
#define SPlat_Events_ControllerInterface_h

#include "Request.h"
#include "Response.h"

namespace SPlat {

    class ControllerInterface {

        virtual Response pop_incoming_response() = 0;

        virtual bool has_outgoing_request() = 0;

        virtual bool has_incoming_response() = 0;

    public:

        virtual void push_outgoing_request(Request) = 0;

        virtual void run() = 0;

        virtual Response await(Request) = 0;

    };

}

#endif