#include "events/DeleteAssetHandler.h"
#include "ControllerInterface.h"
#include "Entrypoint.h"
#include "Event.h"

#include <cereal/archives/json.hpp>

using namespace SPlat::Events;

void DeleteAssetHandler::handle(std::string serialized) {
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
    entrypoint.get_object_model().delete_asset(args.id);
}
