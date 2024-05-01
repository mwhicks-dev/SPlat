#ifndef SPlat_FauxServerController_h
#define SPlat_FauxServerController_h

#include <queue>
#include <mutex>
#include <set>

#include "ControllerInterface.h"

namespace SPlat {

    class FauxServerController : public ControllerInterface {

        std::mutex m;

        std::queue<Request> requests;

        std::queue<Response> responses;

        Response pop_incoming_response() override;

        bool has_outgoing_request() override;

        bool has_incoming_response() override;

        void run_request_thread();

        void run_response_thread();

    public:

        void push_outgoing_request(Request) override;

        void run() override;

        Response await(Request) override;

    };

}

#endif
