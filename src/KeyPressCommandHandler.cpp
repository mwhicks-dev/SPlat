#include "KeyPressCommandHandler.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

void KeyPressCommandHandler::handle(std::string serialized) {
    Args args;
    {
        std::stringstream ss;
        ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // TODO: Insert key into environment held keys
}