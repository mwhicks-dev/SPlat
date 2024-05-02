#include "events/handlers/ServerUpdateAssetHandler.h"
#include "Entrypoint.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

void ServerUpdateAssetHandler::handle(std::string serialized) {
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

    if (asset_properties.get_updated_time() > args.properties.get_updated_time()) return;

    asset_properties.set_fill_color(args.properties.get_fill_color());
    asset_properties.set_position(args.properties.get_position());
    asset_properties.set_size(args.properties.get_size());
}
