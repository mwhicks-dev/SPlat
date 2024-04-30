#include "events/KeyReleaseCommandHandler.h"
#include "Client.h"

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

    Client::get_instance().get_config().get_environment().remove_held_key(args.key);
}