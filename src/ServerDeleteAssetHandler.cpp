#include "events/handlers/ServerDeleteAssetHandler.h"
#include "Entrypoint.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

void ServerDeleteAssetHandler::handle(std::string serialized) {
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

    entrypoint.get_object_model().delete_asset(args.id);
}
