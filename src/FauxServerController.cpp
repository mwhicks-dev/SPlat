#include "events/handlers/CreateCharacterHandler.h"
#include "events/handlers/CreatePlatformHandler.h"
#include "events/handlers/CreateMovingPlatformHandler.h"
#include "Client.h"
#include "Event.h"
#include "IdDto.h"
#include "FauxServerController.h"
#include "ServerDto.h"

#include <cereal/archives/json.hpp>

#include <cppzmq/zmq.hpp>

#include <thread>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat;

void FauxServerController::run_request_thread() {
#ifdef DEBUG
    std::cout << "-> FauxServerController::run_request_thread()" << std::endl;
#endif
    // prepare environment
    EnvironmentInterface& environment 
        = Client::get_instance().get_config().get_environment();
    
    // prepare context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind("tcp://*:9000");

    while (environment.get_running()) {
        // receive client request
        zmq::message_t request_message;
        socket.recv(request_message, zmq::recv_flags::none);

        // deserialize to request obj
        std::string request_string = request_message.to_string();
        Request request;
        {
            std::stringstream ss; ss << request_string;
            cereal::JSONInputArchive iar(ss);
            iar(request);
        }

        push_outgoing_request(request);

        // generate response and serialize
        Response response = await(request);
        std::stringstream ss;
        {
            cereal::JSONOutputArchive oar(ss);
            oar(response);
        }
        zmq::message_t response_message(ss.str());
        socket.send(response_message, zmq::send_flags::none);
    }
#ifdef DEBUG
    std::cout << "<- FauxServerController::run_request_thread" << std::endl;
#endif
}

Response FauxServerController::await(Request request) {
    Response response = { .content_type=Response::ContentType::None, .status=200 };
    const Request::ContentType request_type = request.content_type;
    if (request_type == Request::ContentType::Connect) {
        response.content_type=Response::ContentType::ServerDto;
        ServerDto sd = { .client_id=1, .pub_sub_address="tcp://localhost:9001" };
        std::stringstream ss;
        {
            cereal::JSONOutputArchive oar(ss);
            oar(sd);
        }
        response.body = ss.str();
    } else if (request_type == Request::ContentType::Event) {
        // get event
        Event event;
        {
            std::stringstream ss; ss << request.body;
            cereal::JSONInputArchive iar(ss);
            iar(event);
        }

        if (event.command.type == Events::CreateCharacterHandler
                ::get_type() || event.command.type == Events
                ::CreatePlatformHandler::get_type() || event.command.type
                == Events::CreateMovingPlatformHandler::get_type()) {
            // find latest unused ID
            std::unordered_set<size_t> asset_ids 
                = Client::get_instance().get_object_model().get_ids();
            size_t id = asset_ids.size();
            while (asset_ids.count(id) > 0) id++;
            
            // set generated ID in event
            IdDto id_dto = {
                .id=id
            };
            std::stringstream ss;
            {
                cereal::JSONOutputArchive oar(ss);
                oar(id_dto);
            }

            // update response content
            response.content_type == Response::ContentType::IdDto;
            response.body = ss.str();
        }
    }

    return response;
}

void FauxServerController::run() {
    std::thread t(&FauxServerController::run_request_thread, this);
    t.detach();

    std::thread t2(&FauxServerController::run_publisher_thread, this);
    t2.detach();
}

bool FauxServerController::has_outgoing_request() {
    m.lock();
    auto local = !requests.empty();
    m.unlock();

    return local;
}

Request FauxServerController::pop_outgoing_request() {
    m.lock();
    auto local = requests.front(); requests.pop();
    m.unlock();

    return local;
}

void FauxServerController::run_publisher_thread() {
    zmq::context_t context(2);
    zmq::socket_t socket(context, zmq::socket_type::pub);
    socket.bind("tcp://*:9001");

    EnvironmentInterface& environment = Entrypoint::get_instance().get_config()
        .get_environment();
    
    while (environment.get_running()) {
        if (!has_outgoing_request()) continue;

        Request request = pop_outgoing_request();

        std::stringstream request_stringstream; 
        request_stringstream << "SPlat: ";
        {
            cereal::JSONOutputArchive oar(request_stringstream);
            oar(request);
        }

        zmq::message_t request_message(request_stringstream.str());
        socket.send(request_message, zmq::send_flags::none);
    }
}

void FauxServerController::push_outgoing_request(Request request) {
    m.lock();
    requests.push(request);
    m.unlock();
}
