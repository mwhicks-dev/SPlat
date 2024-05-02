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

        std::priority_queue<Event> outgoing_events;

        Response pop_incoming_response() override;

        Request pop_outgoing_request() override;

        bool has_outgoing_request() override;

        bool has_incoming_response() override;

        std::set<size_t> get_connected_clients();

        void set_connected_clients(std::set<size_t>);

        size_t add_connected_client();

        void remove_connected_client(size_t);

        void run_response_thread();

        void run_publish_thread();

        std::priority_queue<Event> get_outgoing_events();

        void set_outgoing_events(std::priority_queue<Event>);

        Event pop_outgoing_event();

        void push_outgoing_event(Event);

        void run_routine_update_thread();

    public:

        void push_outgoing_request(Request) override;

        void run() override;

        Response await(Request) override;

    };

}

#endif
