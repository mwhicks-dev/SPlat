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
    get_config().get_timing_config().update_framerate_limit(120);

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
    while (environment.get_running()) {
        // update all owned objects
        std::unordered_set<size_t> ids = object_model.get_ids();
        for (size_t id : ids) {
            Model::Asset& asset = object_model.read_asset(id);
            Model::AssetProperties& asset_properties 
                = asset.get_asset_properties();
            if (asset_properties.get_owner() 
                != environment.get_entrypoint_id()) continue;
            try {
                Model::Moving& moving = dynamic_cast<Model::Moving&>(asset);
                moving.update();
                moving.get_moving_properties()
                    .set_last_update(loop_timeline.get_time());
                
                for (size_t other : ids) {
                    if (id == other) continue;

                    SPlat::Model::Asset& other_asset 
                        = object_model.read_asset(other);
                    asset.resolve_collision(other_asset);
                }
            } catch (std::bad_cast&) {}
        }

        time_t next = last_update + 1;
        do {} while (timing.get_display_timeline().get_time() < next);
        last_update = next;
    }

    get_config().get_environment().set_running(false);  // TODO: Get interrupt handler that does this on ^C
}

Server& Server::get_instance() {
    if (instance == nullptr)
        instance = new Server();
    try {
        return *dynamic_cast<Server*>(instance);
    } catch (std::bad_exception&) {
        throw std::invalid_argument("Entrypoint is not of type Server");
    }
}

ConfigInterface& Server::get_config() {
    const std::lock_guard<std::mutex> lock(m);
    return config;
}

Model::ObjectModelInterface& Server::get_object_model() {
    const std::lock_guard<std::mutex> lock(m);
    return object_model;
}

Events::ListenerInterface& Server::get_background_listener() {
    const std::lock_guard<std::mutex> lock(m);
    return background_listener;
}

ControllerInterface& Server::get_controller() {
    const std::lock_guard<std::mutex> lock(m);
    return controller;
}
