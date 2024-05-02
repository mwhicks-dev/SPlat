#ifndef SPlat_ServerController_h
#define SPlat_ServerController_h

#include <queue>
#include <mutex>
#include <set>

#include "ControllerInterface.h"

namespace SPlat {

    class ServerController : public ControllerInterface {

        std::mutex m;

        std::set<size_t> connected_clients;

        std::queue<Request> requests;

        std::queue<Response> responses;

        Response pop_incoming_response() override;

        Request pop_outgoing_request() override;

        bool has_outgoing_request() override;

        bool has_incoming_response() override;

        size_t add_connected_client();

        void remove_connected_client(size_t);

        void run_request_thread();

        void run_response_thread();

    public:

        void push_outgoing_request(Request) override;

        void run() override;

        Response await(Request) override;

    };

}

#endif
