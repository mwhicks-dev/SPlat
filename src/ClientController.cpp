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

        // convert serialized string to zmq::message_t
        zmq::message_t request(serialized.size());
        memcpy(request.data(), serialized.c_str(), serialized.size());
        socket.send(request, zmq::send_flags::none);

        // get reply string
        zmq::message_t reply;
        socket.recv(reply, zmq::recv_flags::none);
        char* serialized_reply = new char(reply.size());
        memcpy(serialized_reply, reply.data(), reply.size());

        // deserialize string into response
        Response resp;
        {
            std::stringstream ss; ss << serialized;
            cereal::JSONInputArchive iar(ss);
            iar(resp);
        }

        // send string to cousin thread
        push_incoming_response(resp);

        delete serialized_reply;
    }
}

void ClientController::run_response_thread() {
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
}

void ClientController::run_subscriber_thread(zmq::context_t* context) {
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
        socket.recv(request, zmq::recv_flags::none);
        char* tmp = new char(request.size());
        memcpy(tmp, request.data(), request.size());

        // deserialize request and push
        Request req;
        {
            std::stringstream ss; ss << tmp;
            cereal::JSONInputArchive iar(ss);
            iar(req);
        }
        push_outgoing_request(req);

        delete tmp;
    }
}

void ClientController::push_incoming_response(Response response) {
    m.lock();
    responses.push(response);
    m.unlock();
}

void ClientController::push_outgoing_request(Request request) {
    m.lock();
    requests.push(request);
    m.unlock();
}

void ClientController::run() {
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
}

Response ClientController::await(Request request) {
    zmq::context_t context(3);
    zmq::socket_t socket(context, zmq::socket_type::req);
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
    char* response_str = new char(response_msg.size());
    memcpy(response_str, response_msg.data(), response_msg.size());

    Response response;
    {
        std::stringstream ss; ss << response_str;
        cereal::JSONInputArchive iar(ss);
        iar(response);
    }

    delete response_str;
    context.close();

    return response;
}