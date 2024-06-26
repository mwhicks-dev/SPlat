#include "events/handlers/ClientCreateMovingPlatformHandler.h"
#include "model/MovingPlatform.h"
#include "ControllerInterface.h"
#include "Entrypoint.h"
#include "Event.h"
#include "IdDto.h"

#include <cereal/archives/json.hpp>

using namespace SPlat::Events;

void ClientCreateMovingPlatformHandler::handle(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> ClientCreateMovingPlatformHandler::handle(" << serialized << ")" << std::endl;
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
    if (event.sender == environment.get_entrypoint_id()) {
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
    }  

    SPlat::Model::Asset* asset = &config.get_asset_factory_config()
        .get_moving_platform_factory().create_asset(args.properties);

    if (event.sender == environment.get_entrypoint_id()) {
        SPlat::Model::MovingPlatform* moving_platform 
            = dynamic_cast<SPlat::Model::MovingPlatform*>(asset);
        moving_platform->get_moving_platform_properties()
            .set_states(args.states);
        moving_platform->get_moving_platform_properties()
            .set_last_state_change(config.get_timing_config()
            .get_anchor_timeline().get_time());
    }
#ifdef DEBUG
    std::cout << "<- ClientCreateMovingPlatformHandler::handle" << std::endl;
#endif
}
