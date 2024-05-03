#include "events/handlers/UpdateAssetHandler.h"
#include "ClientController.h"
#include "Client.h"
#include "Event.h"
#include "FauxServerController.h"
#include "ServerDto.h"
#include "DisconnectDto.h"

#include <cppzmq/zmq.hpp>

#include <cereal/archives/json.hpp>

#include <sstream>
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

    // process events during runtime
    while (environment.get_running()) {
        if (!has_outgoing_request()) continue;

        Request curr = pop_outgoing_request();
        Response current_response = await(curr);

        // send string to cousin thread
        push_incoming_response(current_response);
    }

    // disconnect from server
    std::stringstream ss;
    DisconnectDto disconnect_dto = {
        .entrypoint_id=environment.get_entrypoint_id(),
    };
    {
        cereal::JSONOutputArchive oar(ss);
        oar(disconnect_dto);
    };
    Request disconnect_request = {
        .content_type=Request::ContentType::Disconnect,
        .body=ss.str(),
    };

    size_t status;
    do {
        status = await(disconnect_request).status;
    } while (status != 200);
#ifdef DEBUG
    std::cout << "<- ClientController::run_request_thread" << std::endl;
#endif
}

void ClientController::run_response_thread() {
#ifdef DEBUG
    std::cout << "-> ClientController::run_response_thread()" << std::endl;
#endif

    // prepare client
    Client& cli = Client::get_instance();
    Events::ListenerInterface& listener =
        cli.get_background_listener();
    EnvironmentInterface& environment 
        = cli.get_config().get_environment();
    
    while (environment.get_running()) {
        if (!has_incoming_response()) continue;

        Response curr = pop_incoming_response();

        // handle later
    }
#ifdef DEBUG
    std::cout << "<- ClientController::run_response_thread()" << std::endl;
#endif
}

bool preprocess_event(Event event) {
    if (event.sender != 0) return true;
    
    if (event.command.type != Events::UpdateAssetHandler::get_type()) return true;

    Events::UpdateAssetHandler::Args args;
    {
        std::stringstream args_ss;
        args_ss << event.command.args;
        cereal::JSONInputArchive iar(args_ss);
        iar(args);
    }

    if (args.properties.get_owner() == Client::get_instance().get_config().get_environment().get_entrypoint_id()) return false;

    return true;
}

void ClientController::run_subscriber_thread() {
#ifdef DEBUG
    std::cout << "-> ClientController::run_subscriber_thread()" << std::endl;
#endif
    // prepare environment
    Client& client = Client::get_instance();
    Events::ListenerInterface& background_listener 
        = client.get_background_listener();
    EnvironmentInterface& environment 
        = client.get_config().get_environment();
    const char* filter = "SPlat: ";

    // prepare context, socket, and filter
    zmq::socket_t socket(context, zmq::socket_type::sub);
    socket.connect(environment.get_pub_sub_addres());
    socket.set(zmq::sockopt::subscribe, filter);

    while (environment.get_running()) {
        // get request string
        zmq::message_t request;
        std::string serialized;
        try {
            socket.recv(request, zmq::recv_flags::none);
        } catch (zmq::error_t&) {
            if (!environment.get_running()) return;
            std::cerr << "Could not connect to server" << std::endl;
            std::cerr << "Shutting down..." << std::endl;
            continue;
        }

        // deserialize request and push
        Request req;
        {
            // std::stringstream ss; ss << request.to_string()
            //     .substr(strlen("SPlat: "));
            std::stringstream ss;
            ss << request.to_string().substr(strlen("SPlat: "));
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
            if (sender != environment.get_entrypoint_id()) {
                if (preprocess_event(e))
                    background_listener.push_event(e);
                push_outgoing_request(req);
            }
        }
    }

    socket.close();
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
    
    // start subscriber thread
    std::thread subscriber_thread(&ClientController::run_subscriber_thread, this);

    // detach
    request_thread.detach();
    response_thread.detach();
    subscriber_thread.detach();
#ifdef DEBUG
    std::cout << "<- ClientController::run()" << std::endl;
#endif
}

Response ClientController::await(Request request) {
    zmq::socket_t socket(context, zmq::socket_type::req);
    
    timeval retry_time = {
        .tv_sec=3,
        .tv_usec=0
    };
    
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
    m.lock();
    socket.send(msg, zmq::send_flags::none);

    zmq::message_t response_msg;
    socket.recv(response_msg, zmq::recv_flags::none);
    m.unlock();
    std::string response_str = response_msg.to_string();

    Response response;
    {
        std::stringstream ss; ss << response_str;
        cereal::JSONInputArchive iar(ss);
        iar(response);
    }

    socket.close();

    return response;
}

ClientController::ClientController() {
    context = zmq::context_t(1);
}

ClientController::~ClientController() {
    context.close();
}
