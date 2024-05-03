#include "events/handlers/ServerCreatePlatformHandler.h"
#include "events/handlers/SpawnAssetHandler.h"
#include "Entrypoint.h"
#include "IdDto.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

void ServerCreatePlatformHandler::handle(std::string serialized) {
    Entrypoint& entrypoint = Entrypoint::get_instance();
    EnvironmentInterface& environment = entrypoint.get_config().get_environment();

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

    IdDto id_dto = {id};
    std::stringstream id_dto_ss;
    {
        cereal::JSONOutputArchive oar(id_dto_ss);
        oar(id_dto);
    }
    environment.set_context(event.context, id_dto_ss.str());

    SPlat::Model::Asset& asset = entrypoint.get_config()
        .get_asset_factory_config().get_platform_factory()
        .create_asset(args.properties);

    SpawnAssetHandler::Args spawn_args = {
        .asset_id=id
    };
    std::stringstream spawn_args_ss;
    {
        cereal::JSONOutputArchive oar(spawn_args_ss);
        oar(spawn_args);
    }

    // spawn newly created asset
    Event spawn_event {
        .command = {
            .priority=-2,
            .type=SpawnAssetHandler::get_type(),
            .args=spawn_args_ss.str()
        },
        .sender=environment.get_entrypoint_id()
    };
    entrypoint.get_background_listener().push_event(spawn_event);
}