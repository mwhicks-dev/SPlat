#include "events/handlers/ServerCreateCharacterHandler.h"
#include "events/handlers/ServerCreatePlatformHandler.h"
#include "events/handlers/ServerCreateMovingPlatformHandler.h"
#include "events/handlers/ServerReadAssetHandler.h"
#include "events/handlers/ServerUpdateAssetHandler.h"
#include "events/handlers/ServerDeleteAssetHandler.h"
#include "events/OrderedPriorityListener.h"
#include "model/UnorderedMapObjectModel.h"
#include "ServerController.h"
#include "ClientConfig.h"
#include "Server.h"

using namespace SPlat;

Server::Server() : 
config(*new ClientConfig()),
controller(*new ServerController()),
object_model(*new Model::UnorderedMapObjectModel()),
background_listener(*new Events::OrderedPriorityListener()) {
    get_config().get_timing_config().update_framerate_limit(30);

    // set listener handlers
    background_listener.set_handler(Events::ServerCreateCharacterHandler::get_type(),
        *new Events::ServerCreateCharacterHandler());
    background_listener.set_handler(Events::ServerCreatePlatformHandler::get_type(),
        *new Events::ServerCreatePlatformHandler());
    background_listener.set_handler(Events::ServerCreateMovingPlatformHandler
        ::get_type(), *new Events::ServerCreateMovingPlatformHandler());
    background_listener.set_handler(Events::ServerReadAssetHandler::get_type(), *new
        Events::ServerReadAssetHandler());
    background_listener.set_handler(Events::ServerUpdateAssetHandler::get_type(), 
        *new Events::ServerUpdateAssetHandler());
    background_listener.set_handler(Events::ServerDeleteAssetHandler::get_type(), 
        *new Events::ServerDeleteAssetHandler());
}

void Server::start() {
    ConfigInterface& config = get_config();
    EnvironmentInterface& environment = config.get_environment();
    TimingConfigInterface& timing = config.get_timing_config();
    Model::ObjectModelInterface& object_model = get_object_model();
    LocalTimeline& loop_timeline = timing.get_display_timeline();
    
    environment.set_req_rep_address("tcp://localhost:5555");
    environment.set_pub_sub_addres("tcp://localhost:5556");
    
    controller.run();
    background_listener.run();

    loop_timeline.unpause();

    time_t last_update = timing.get_display_timeline().get_time();
    while (1) {
        std::unordered_set<size_t> ids = object_model.get_ids();
        std::vector<SPlat::Model::Moving*> moving_vector;
        for (size_t id : ids) {
            try {
                SPlat::Model::Moving& moving = dynamic_cast
                    <SPlat::Model::Moving&>(object_model.read_asset(id));
                if (moving.get_asset_properties().get_owner() 
                        == environment.get_entrypoint_id())
                    moving_vector.push_back(&moving);
            } catch (std::bad_cast&) {}
        }

        for (SPlat::Model::Moving* moving : moving_vector) {
            moving->update();
            moving->get_moving_properties().set_last_update(loop_timeline
                .get_time());
            
            size_t id = moving->get_asset_properties().get_id();
            for (size_t other : ids) {
                if (id == other) continue;

                SPlat::Model::Asset& other_asset = object_model.read_asset(other);
                moving->resolve_collision(other_asset);
            }
        }
    }

    get_config().get_environment().set_running(false);  // TODO: Get interrupt handler that does this on ^C
}

Server& Server::get_instance() {
    m.lock();
    if (instance == nullptr) {
        instance = new Server();
    }
    m.unlock();
    Server* local = nullptr;
    try {
        m.lock();
        local = dynamic_cast<Server*>(instance);
    } catch (std::bad_exception&) {}
    m.unlock();
    if (local == nullptr) {
        throw std::invalid_argument("Entrypoint is not of type Server");
    }

    return *local;
}

ConfigInterface& Server::get_config() {
    m.lock();
    ConfigInterface& local = config;
    m.unlock();

    return local;
}

Model::ObjectModelInterface& Server::get_object_model() {
    m.lock();
    Model::ObjectModelInterface& local = object_model;
    m.unlock();

    return local;
}

Events::ListenerInterface& Server::get_background_listener() {
    m.lock();
    Events::ListenerInterface& local = background_listener;
    m.unlock();

    return local;
}

ControllerInterface& Server::get_controller() {
    m.lock();
    ControllerInterface& local = controller;
    m.unlock();

    return local;
}
