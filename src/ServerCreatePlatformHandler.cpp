#include "events/handlers/ServerCreatePlatformHandler.h"
#include "Entrypoint.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

void ServerCreatePlatformHandler::handle(std::string serialized) {
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

    std::unordered_set<size_t> asset_ids = entrypoint.get_object_model()
        .get_ids();
    size_t id = asset_ids.size();
    while (asset_ids.count(id) > 0) id++;

    args.properties.set_id(id);

    // TODO: Provide way for IdDto to be returned by server
    // Event::context: size_t, default 0, if 0 then no context (like nullptr)
    // Environment contains mapping of Event::context to string, string denotes
    // output (ie. IdDto, or AssetProperties for ServerGetAssetHandler)

    SPlat::Model::Asset& asset = entrypoint.get_config()
        .get_asset_factory_config().get_platform_factory()
        .create_asset(args.properties);
}