#include "KeyReleaseCommandHandler.h"

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

    // TODO: Remove key from environment held keys
}