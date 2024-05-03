#include "events/handlers/ServerCreateCharacterHandler.h"
#include "model/Character.h"
#include "Entrypoint.h"
#include "IdDto.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

void ServerCreateCharacterHandler::handle(std::string serialized) {
    Entrypoint& entrypoint = Entrypoint::get_instance();
    ConfigInterface& config = entrypoint.get_config();
    EnvironmentInterface& environment = config.get_environment();

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

    SPlat::Model::Asset& asset = config.get_asset_factory_config()
        .get_character_factory().create_asset(args.properties);
    
    if (event.sender == environment.get_entrypoint_id() 
            && args.set_controlled) {
        SPlat::Model::Asset* asset_ptr = &asset;
        SPlat::Model::Character* character 
            = dynamic_cast<SPlat::Model::Character*>(asset_ptr);
    }
}