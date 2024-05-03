#include "events/handlers/ReadAssetHandler.h"
#include "events/handlers/UpdateAssetHandler.h"
#include "events/handlers/DeleteAssetHandler.h"
#include "IdDto.h"
#include "Server.h"
#include "ServerController.h"
#include "ServerDto.h"

#include <cereal/archives/json.hpp>

#include <cppzmq/zmq.hpp>

#include <sstream>
#include <thread>

using namespace SPlat;

Response ServerController::pop_incoming_response() {
    const std::lock_guard<std::mutex> lock(m);
    auto local = responses.front(); responses.pop();
    
    return local;
}

Request ServerController::pop_outgoing_request() {
    const std::lock_guard<std::mutex> lock(m);
    auto local = requests.front(); requests.pop();
    
    return local;
}

bool ServerController::has_outgoing_request() {
    const std::lock_guard<std::mutex> lock(m);
    return !requests.empty();
}

bool ServerController::has_incoming_response() {
    const std::lock_guard<std::mutex> lock(m);
    return !responses.empty();
}

std::set<size_t> ServerController::get_connected_clients() {
    const std::lock_guard<std::mutex> lock(m);
    return connected_clients;
}

void ServerController::set_connected_clients(std::set<size_t> connected_clients) {
    const std::lock_guard<std::mutex> lock(m);
    this->connected_clients = connected_clients;
}

size_t ServerController::add_connected_client() {
    auto local = get_connected_clients();
    size_t client_id = local.size() + 1;
    while (local.count(client_id) > 0) client_id++;
    local.insert(client_id);
    set_connected_clients(local);

    return client_id;
}

void ServerController::remove_connected_client(size_t client) {
    auto local = get_connected_clients();
    local.erase(client);
    set_connected_clients(local);
}

void ServerController::push_outgoing_request(Request request) {
    const std::lock_guard<std::mutex> lock(m);
    requests.push(request);
}

Response ServerController::await(Request request) {
    Server& server = Server::get_instance();
    ConfigInterface& config = server.get_config();
    EnvironmentInterface& environment = config.get_environment();

    Response response = { 
        .content_type=Response::ContentType::None, 
        .status=200
    };
    try {
        if (request.content_type == Request::ContentType::Connect) {
            // add new client and return serialized ID in response
            response.content_type = Response::ContentType::ServerDto;
            size_t client_id = add_connected_client();
            ServerDto server_dto = {
                .client_id=client_id,
                .pub_sub_address=environment.get_pub_sub_addres()
            };
            std::stringstream server_dto_ss;
            {
                cereal::JSONOutputArchive oar(server_dto_ss);
                oar(server_dto);
            }
            response.body = server_dto_ss.str();
        } else if (request.content_type == Request::ContentType::Disconnect) {
            IdDto id_dto;
            {
                std::stringstream id_dto_ss;
                id_dto_ss << request.body;
                cereal::JSONInputArchive iar(id_dto_ss);
                iar(id_dto);
            }

            Model::ObjectModelInterface& object_model = server.get_object_model();
            Events::ListenerInterface& listener = server.get_background_listener();

            Event prototype = {
                .command = {
                    .priority=-1,
                    .type=Events::DeleteAssetHandler::get_type(),
                    .args=""
                },
                .sender=environment.get_entrypoint_id()
            };

            std::unordered_set<size_t> asset_ids = object_model.get_ids();
            for (size_t id : asset_ids) {
                Model::Asset& asset = object_model.read_asset(id);
                if (asset.get_asset_properties().get_owner() == id_dto.id) {
                    // push event to delete asset owned by disconnected client
                    Events::DeleteAssetHandler::Args args = {id};
                    std::stringstream delete_asset_args_ss;
                    {
                        cereal::JSONOutputArchive oar(delete_asset_args_ss);
                        oar(args);
                    }
                    Event instance = prototype;
                    instance.command.args = delete_asset_args_ss.str();
                    listener.push_event(instance);
                    push_outgoing_event(instance);
                }
            }
        } else if (request.content_type == Request::ContentType::Event) {
            Events::ListenerInterface& listener = server.get_background_listener();
            Event event;
            {
                std::stringstream event_ss;
                event_ss << request.body;
                cereal::JSONInputArchive iar(event_ss);
                iar(event);
            }

            try {
                listener.await(event);
                response.body = environment.get_context(event.context);
            } catch (std::exception& e) {
                response.status = 400;
                response.body = e.what();  // TODO replace with event exception string
            }
            // push_outgoing_event(event);
        }
    } catch (std::exception& e) {
        response.status = 500;
        response.body = e.what();
    }
    
    return response;
}

void ServerController::run_response_thread() {
    EnvironmentInterface& environment 
        = Server::get_instance().get_config().get_environment();
    
    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind("tcp://*:5555");

    while (environment.get_running()) {
        // receive request message
        zmq::message_t request_message;
        socket.recv(request_message, zmq::recv_flags::none);

        // deserialize message as Request
        Request request;
        {
            std::stringstream request_ss;
            request_ss << request_message.to_string();
            cereal::JSONInputArchive iar(request_ss);
            iar(request);
        }

        // await response
        Response response = await(request);

        // serialize Response as message
        std::stringstream response_ss;
        {
            cereal::JSONOutputArchive oar(response_ss);
            oar(response);
        }
        zmq::message_t response_message(response_ss.str());

        // send back response message
        socket.send(response_message, zmq::send_flags::none);
    }

    socket.close();
}

void ServerController::set_outgoing_events(std::priority_queue<Event> outgoing_events) {
    const std::lock_guard<std::mutex> lock(m);
    this->outgoing_events = outgoing_events;
}

std::priority_queue<Event> ServerController::get_outgoing_events() {
    const std::lock_guard<std::mutex> lock(m);
    return outgoing_events;
}

Event ServerController::pop_outgoing_event() {
    auto pq = get_outgoing_events();
    auto event = pq.top(); pq.pop();
    set_outgoing_events(pq);
    return event;
}

void ServerController::push_outgoing_event(Event event) {
    auto pq = get_outgoing_events();
    pq.push(event);
    set_outgoing_events(pq);
}

void ServerController::run_publish_thread() {
    EnvironmentInterface& environment 
        = Server::get_instance().get_config().get_environment();

    zmq::socket_t socket(context, zmq::socket_type::pub);
    socket.bind("tcp://*:5556");

    while (environment.get_running()) {
        if (get_outgoing_events().empty()) continue;

        Event event = pop_outgoing_event();
        std::stringstream event_ss;
        {
            cereal::JSONOutputArchive oar(event_ss);
            oar(event);
        }
        Request request = {
            .content_type=Request::ContentType::Event,
            .body=event_ss.str()
        };
        std::stringstream request_ss;
        request_ss << "SPlat: ";
        {
            cereal::JSONOutputArchive oar(request_ss);
            oar(request);
        }
        socket.send(zmq::message_t(request_ss.str()), zmq::send_flags::none);
    }

    socket.close();
}

void ServerController::run() {
    std::thread response_thread(&ServerController::run_response_thread, this);
    std::thread publisher_thread(&ServerController::run_publish_thread, this);
    std::thread routine_update_thread(&ServerController::run_routine_update_thread, this);

    response_thread.detach();
    publisher_thread.detach();
    routine_update_thread.detach();
}

void ServerController::run_routine_update_thread() {
    Server& server = Server::get_instance();
    EnvironmentInterface& environment = server.get_config().get_environment();
    Model::ObjectModelInterface& object_model 
        = server.get_object_model();
    
    Event prototype = {
        .command={
            .priority=-1,
            .type=Events::UpdateAssetHandler::get_type()
        },
        .sender=environment.get_entrypoint_id()
    };
    
    while (environment.get_running()) {
        // sleep for variable time -- for now 1s
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // push new updateassetevent with all id
        std::unordered_set<size_t> asset_ids = object_model.get_ids();
        for (size_t asset_id : asset_ids) {
            // format args for specific asset
            Model::Asset& curr = object_model.read_asset(asset_id);
            Event asset_event = prototype;
            Events::UpdateAssetHandler::Args args = {
                .id=asset_id,
                .properties=curr.get_asset_properties()
            };

            // serialize args to command args
            std::stringstream args_ss;
            {
                cereal::JSONOutputArchive oar(args_ss);
                oar(args);
            }
            asset_event.command.args = args_ss.str();

            // push event to outgoing
            push_outgoing_event(asset_event);
        }
    }
}

ServerController::ServerController() {
    context = zmq::context_t(2);
}

ServerController::~ServerController() {
    context.close();
}
