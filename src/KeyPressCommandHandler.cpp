#include "events/KeyPressCommandHandler.h"
#include "Entrypoint.h"
#include "Event.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

void KeyPressCommandHandler::handle(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> KeyPressCommandHandler::handle(" << serialized << ")" << std::endl;
#endif
    SPlat::Event event;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(event);
    }

    Args args;
    {
        std::stringstream iss; iss << event.command.args;
        cereal::JSONInputArchive iar(iss);
        iar(args);
    }

    Entrypoint::get_instance().get_config().get_environment().add_held_key(args.key);
#ifdef DEBUG
    std::cout << "<- KeyPressCommandHandler::handle" << std::endl;
#endif
}