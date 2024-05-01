#include "events/UpdateAssetHandler.h"
#include "ControllerInterface.h"
#include "Entrypoint.h"
#include "Event.h"

#include <cereal/archives/json.hpp>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Events;

void UpdateAssetHandler::handle(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> UpdateAssetHandler::handle(" << serialized << ")" << std::endl;
#endif
    Entrypoint& entrypoint = Entrypoint::get_instance();
    ConfigInterface& config = entrypoint.get_config();
    EnvironmentInterface& environment = config.get_environment();
    Timeline& anchor = config.get_timing_config().get_anchor_timeline();

    // recreate event from details
    SPlat::Event event = {
        .event_time=anchor.get_time(), 
        .command={
            .priority=0,
            .type=get_type(),
            .args=serialized
        },
        .sender=environment.get_entrypoint_id()
    };

    // send to server as request
    std::stringstream ss;
    {
        cereal::JSONOutputArchive oar(ss);
        oar(event);
    }
    Request request = {
        .content_type=Request::ContentType::Event,
        .body=ss.str()
    };
    ControllerInterface& ctl = entrypoint.get_controller();
    Response response = ctl.await(request);

    if (response.status != 200) {
        std::cerr << "Could not update asset (" << response.status << "):" << std::endl;
        std::cerr << response.body << std::endl;
        throw std::logic_error("");  // TODO create some TCPException class
    }

    // perform update client-side
    Args args;
    {
        std::stringstream ssi; ssi << serialized;
        cereal::JSONInputArchive iar(ssi);
        iar(args);
    }
    sf::Vector2f diff = entrypoint.get_object_model().read_asset(args.id)
        .get_asset_properties().get_position() - args.properties
        .get_position();
    config.get_environment().get_standing_config().push_update_to_children(args.id, diff);
#ifdef DEBUG
    std::cout << "<- UpdateAssetHandler::handle" << std::endl;
#endif
}
