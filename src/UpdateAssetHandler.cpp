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
        std::cerr << "Could not update asset (" << response.status << "):" << std::endl;
        std::cerr << response.body << std::endl;
        throw std::logic_error("");  // TODO create some TCPException class
    }

    // perform update client-side
    sf::Vector2f diff = entrypoint.get_object_model().read_asset(args.id)
        .get_asset_properties().get_position() - args.properties
        .get_position();
    config.get_environment().get_standing_config().push_update_to_children(args.id, diff);
#ifdef DEBUG
    std::cout << "<- UpdateAssetHandler::handle" << std::endl;
#endif
}
