#ifndef SPlat_ClientController_h
#define SPlat_ClientController_h

#include "ControllerInterface.h"

#include <cppzmq/zmq.hpp>

#include <queue>
#include <mutex>

namespace SPlat {

    class ClientController : public ControllerInterface {

        std::mutex m;

        zmq::context_t context;

        std::queue<Request> requests;

        std::queue<Response> responses;

        Request pop_outgoing_request() override;

        Response pop_incoming_response() override;

        bool has_outgoing_request() override;

        bool has_incoming_response() override;

        void run_request_thread();

        void run_subscriber_thread();

        void run_response_thread();
        
        void push_incoming_response(Response);

        zmq::context_t get_context();

    public:

        void push_outgoing_request(Request) override;

        void run() override;

        Response await(Request) override;

        ClientController();

        ~ClientController();

    };

}

#endif
