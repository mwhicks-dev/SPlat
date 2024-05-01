#include "ClientController.h"
#include "Client.h"
#include "Event.h"
#include "FauxServerController.h"
#include "ServerDto.h"

#include <cppzmq/zmq.hpp>

#include <cereal/archives/json.hpp>

#include <thread>

using namespace SPlat;

Request ClientController::pop_outgoing_request() {
    m.lock();
    auto local = requests.front(); requests.pop();
    m.unlock();

    return local;
}

Response ClientController::pop_incoming_response() {
    m.lock();
    auto local = responses.front(); responses.pop();
    m.unlock();

    return local;
}

bool ClientController::has_outgoing_request() {
    m.lock();
    auto local = !requests.empty();
    m.unlock();

    return local;
}

bool ClientController::has_incoming_response() {
    m.lock();
    auto local = !responses.empty();
    m.unlock();

    return local;
}

void ClientController::run_request_thread() {
#ifdef DEBUG
    std::cout << "-> ClientController::run_request_thread()" << std::endl;
#endif
    // prepare client environment
    EnvironmentInterface& environment 
        = Client::get_instance().get_config().get_environment();

    // prepare context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::req);

    socket.connect(environment.get_req_rep_addres());  // Assumed set by this point; handle in main if not

    while (environment.get_running()) {
        if (!has_outgoing_request()) continue;

        // get request string
        Request curr = pop_outgoing_request();
        std::stringstream ss;
        {
            cereal::JSONOutputArchive oar(ss);
            oar(curr);
        }
        std::string serialized = ss.str();
        ss.clear(); ss.str("");

        // convert serialized string to zmq::message_t
        zmq::message_t request(serialized.size());
        memcpy(request.data(), serialized.c_str(), serialized.size());
        socket.send(request, zmq::send_flags::none);

        // get reply string
        zmq::message_t reply;
        socket.recv(reply, zmq::recv_flags::none);
        std::string serialized_reply = reply.to_string();

        // deserialize string into response
        Response resp;
        {
            std::stringstream ss; ss << serialized_reply;
            cereal::JSONInputArchive iar(ss);
            iar(resp);
        }

        // send string to cousin thread
        push_incoming_response(resp);
    }
#ifdef DEBUG
    std::cout << "<- ClientController::run_request_thread" << std::endl;
#endif
}

void ClientController::run_response_thread() {
#ifdef DEBUG
    std::cout << "-> ClientController::run_response_thread()" << std::endl;
#endif
    // start subscriber thread
    zmq::context_t * subscriber_context = new zmq::context_t(2);
    std::thread subscriber_thread(&ClientController::run_subscriber_thread, 
        this, subscriber_context);
    subscriber_thread.detach();

    // prepare client
    Client& cli = Client::get_instance();
    Events::ListenerInterface& listener =
        cli.get_background_listener();
    EnvironmentInterface& environment 
        = cli.get_config().get_environment();
    
    while (environment.get_running()) {
        if (!has_incoming_response()) continue;

        Response curr = pop_incoming_response();

        const Response::ContentType type = curr.content_type;
        if (type == Response::ContentType::Event) {
            Event e;
            {
                std::stringstream ss; ss << curr.body;
                cereal::JSONInputArchive iar(ss);
                iar(e);
            }
            listener.push_command(e.command);
        }
    }

    subscriber_context->close();
#ifdef DEBUG
    std::cout << "<- ClientController::run_response_thread()" << std::endl;
#endif
}

void ClientController::run_subscriber_thread(zmq::context_t* context) {
#ifdef DEBUG
    std::cout << "-> ClientController::run_subscriber_thread(zmq::context_t*)" << std::endl;
#endif
    // prepare environment
    EnvironmentInterface& environment 
        = Client::get_instance().get_config().get_environment();
    const char* filter = "SPlat: ";

    // prepare context, socket, and filter
    zmq::socket_t socket(*context, zmq::socket_type::sub);
    socket.connect(environment.get_pub_sub_addres());
    socket.set(zmq::sockopt::subscribe, filter);

    while (environment.get_running()) {
        // get request string
        zmq::message_t request;
        std::string serialized;
        try {
            socket.recv(request, zmq::recv_flags::none);
        } catch (zmq::error_t&) {
            std::cerr << "Could not connect to server" << std::endl;
            std::cerr << "Shutting down..." << std::endl;
            continue;
        }
        char* tmp = new char(request.size());
        memcpy(tmp, request.data(), request.size());

        // deserialize request and push
        Request req;
        {
            std::stringstream ss; ss << tmp;
            cereal::JSONInputArchive iar(ss);
            iar(req);
        }
        size_t sender = 0;  // server by default
        if (req.content_type == Request::ContentType::Event) {
            // check if sender is self
            Event e;
            {
                std::stringstream ss; ss << req.body;
                cereal::JSONInputArchive iar(ss);
                iar(e);
            }
            sender = e.sender;
        }

        if (sender != environment.get_entrypoint_id())
            push_outgoing_request(req);

        delete tmp;
    }
#ifdef DEBUG
    std::cout << "<- ClientController::run_subscriber_thread" << std::endl;
#endif
}

void ClientController::push_incoming_response(Response response) {
#ifdef DEBUG
    std::cout << "-> ClientController::push_incoming_response(Response)" << std::endl;
#endif
    m.lock();
    responses.push(response);
    m.unlock();
#ifdef DEBUG
    std::cout << "<- ClientController::push_incoming_response" << std::endl;
#endif
}

void ClientController::push_outgoing_request(Request request) {
#ifdef DEBUG
    std::cout << "-> ClientController::push_outgoing_request(Request)" << std::endl;
#endif
    m.lock();
    requests.push(request);
    m.unlock();
#ifdef DEBUG
    std::cout << "<- ClientController::push_outgoing_request" << std::endl;
#endif
}

void ClientController::run() {
#ifdef DEBUG
    std::cout << "-> ClientController::run()" << std::endl;
#endif
    // ensure server address defined; if not, create new faux
    EnvironmentInterface& environment = Client::get_instance().get_config()
        .get_environment();
    if (environment.get_req_rep_addres() == "") {
        ControllerInterface * faux = new FauxServerController();
        faux->run();
        environment.set_req_rep_address("tcp://localhost:9000");
    }

    // request server connect
    Request join_request = {
        .content_type=Request::ContentType::Connect
    };
    Response join_response = await(join_request);
    if (join_response.status != 200) {
        std::cerr << "Could not connect to server (";
        std::cerr << join_response.status << "): " << std::endl;
        std::cerr << join_response.body << std::endl;
        throw std::exception();  // TODO make request error class
    } else {
        ServerDto sd;
        {
            std::stringstream ss; ss << join_response.body;
            cereal::JSONInputArchive iar(ss);
            iar(sd);
        }
        environment.set_entrypoint_id(sd.client_id);
        environment.set_pub_sub_addres(sd.pub_sub_address);
    }
    
    // start req/rep threads
    std::thread request_thread(&ClientController::run_request_thread, this);
    std::thread response_thread(&ClientController::run_response_thread, this);

    // detach
    request_thread.detach();
    response_thread.detach();
#ifdef DEBUG
    std::cout << "<- ClientController::run()" << std::endl;
#endif
}

Response ClientController::await(Request request) {
    zmq::context_t context(3);
    zmq::socket_t socket(context, zmq::socket_type::req);
    
    EnvironmentInterface& environment = Client::get_instance().get_config().get_environment();

    std::string req_rep_address;
    do {
        req_rep_address = environment.get_req_rep_addres();
    } while (req_rep_address == "");

    socket.connect(Client::get_instance().get_config().get_environment()
        .get_req_rep_addres());

    std::stringstream ss;
    {
        cereal::JSONOutputArchive oar(ss);
        oar(request);
    }
    std::string serialized = ss.str();

    zmq::message_t msg(serialized.size());
    memcpy(msg.data(), serialized.c_str(), serialized.size());
    socket.send(msg, zmq::send_flags::none);

    zmq::message_t response_msg;
    socket.recv(response_msg, zmq::recv_flags::none);
    std::string response_str = response_msg.to_string();

    Response response;
    {
        std::stringstream ss; ss << response_str;
        cereal::JSONInputArchive iar(ss);
        iar(response);
    }

    socket.close();
    context.close();

    return response;
}
