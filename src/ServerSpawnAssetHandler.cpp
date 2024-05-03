#include "events/handlers/ServerSpawnAssetHandler.h"
#include "Event.h"
#include "Server.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

void ServerSpawnAssetHandler::handle(std::string serialized) {
    Server& server = Server::get_instance();
    EnvironmentInterface& environment = server.get_config().get_environment();
    
    Event event;
    {
        std::stringstream event_ss;
        event_ss << serialized;
        cereal::JSONInputArchive iar(event_ss);
        iar(event);
    }

    Args args;
    {
        std::stringstream args_ss;
        args_ss << event.command.args;
        cereal::JSONInputArchive iar(args_ss);
        iar(args);
    }



    Model::Asset& asset = server.get_object_model().read_asset(args.asset_id);
    Model::AssetProperties& asset_properties = asset.get_asset_properties();
    if (event.sender != asset_properties.get_owner() && event.sender != 
            server.get_config().get_environment().get_entrypoint_id())
        throw std::invalid_argument("Cannot spawn event you do not own");
    
    if (asset_properties.get_loaded()) return;

    asset_properties.set_loaded(true);
    asset_properties.set_position(server.get_spawnpoint()
        .get_asset_properties().get_position());
}
