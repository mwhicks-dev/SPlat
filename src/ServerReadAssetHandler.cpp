#include "events/handlers/ServerReadAssetHandler.h"
#include "Entrypoint.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

void ServerReadAssetHandler::handle(std::string serialized) {
    Entrypoint& entrypoint = Entrypoint::get_instance();

    SPlat::Event event;
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

    Model::Asset& asset = entrypoint.get_object_model().read_asset(args.id);
    Model::AssetProperties& asset_properties = asset.get_asset_properties();

    std::stringstream asset_properties_ss;
    {
        cereal::JSONOutputArchive oar(asset_properties_ss);
        oar(asset_properties);
    }
    entrypoint.get_config().get_environment().set_context(event.context,
        asset_properties_ss.str());
}
