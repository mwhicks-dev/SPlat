#include "events/KeyPressCommandHandler.h"
#include "Entrypoint.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

void KeyPressCommandHandler::handle(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> KeyPressCommandHandler::handle(" << serialized << ")" << std::endl;
#endif
    Args args;
    {
        std::stringstream ss;
        ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    Entrypoint::get_instance().get_config().get_environment().add_held_key(args.key);
#ifdef DEBUG
    std::cout << "<- KeyPressCommandHandler::handle" << std::endl;
#endif
}