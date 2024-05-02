#include "events/handlers/CreatePlatformHandler.h"
#include "ControllerInterface.h"
#include "Entrypoint.h"
#include "Event.h"
#include "IdDto.h"

#include <cereal/archives/json.hpp>

using namespace SPlat::Events;

void CreatePlatformHandler::handle(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> CreatePlatformHandler::handle(" << serialized << ")" << std::endl;
#endif
    Entrypoint& entrypoint = Entrypoint::get_instance();
    ConfigInterface& config = entrypoint.get_config();
    EnvironmentInterface& environment = config.get_environment();
    Timeline& anchor = config.get_timing_config().get_anchor_timeline();

    // recreate event from details
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
    
    // send to server as request
    Request request = {
        .content_type=Request::ContentType::Event,
        .body=serialized
    };

    ControllerInterface& ctl = entrypoint.get_controller();
    Response response = ctl.await(request);

    if (response.status != 200) {
        std::cerr << "Could not create character (" << response.status << "):" << std::endl;
        std::cerr << response.body << std::endl;
        throw std::logic_error("");  // TODO create some TCPException class
    }

    SPlat::IdDto id_dto;
    {
        std::stringstream iss; iss << response.body;
        cereal::JSONInputArchive iar(iss);
        iar(id_dto);
    }

    args.properties.set_id(id_dto.id);

    SPlat::Model::Asset* asset = &config.get_asset_factory_config()
        .get_platform_factory().create_asset(args.properties);
#ifdef DEBUG
    std::cout << "<- CreatePlatformHandler::handle" << std::endl;
#endif
}
