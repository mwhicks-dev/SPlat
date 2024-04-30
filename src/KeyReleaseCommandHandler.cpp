#include "events/KeyReleaseCommandHandler.h"
#include "Entrypoint.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

void KeyReleaseCommandHandler::handle(std::string serialized) {
    Args args;
    {
        std::stringstream ss;
        ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    Entrypoint::get_instance().get_config().get_environment().remove_held_key(args.key);
}