#include "events/GetAssetHandler.h"
#include "events/CreatePlatformHandler.h"
#include "ControllerInterface.h"
#include "Entrypoint.h"
#include "Event.h"

#include <cereal/archives/json.hpp>

using namespace SPlat::Events;

void GetAssetHandler::handle(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> GetAssetHandler::handle(" << serialized << ")" << std::endl;
#endif
    /// used to query an asset for the first time; server returns creator
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
        std::cerr << "Could not retrieve asset (" << response.status << "):" << std::endl;
        std::cerr << response.body << std::endl;
        throw std::logic_error("");  // TODO create some TCPException class
    }
    
    SPlat::Event server_event;
    {
        std::stringstream iss; iss << response.body;
        cereal::JSONInputArchive iar(iss);
        iar(server_event);
    }

    CreatePlatformHandler::Args args;  // all creators' properties same so we can get away with this
    {
        std::stringstream iss; iss << server_event.command.args;
        cereal::JSONInputArchive iar(iss);
        iar(args);
    }

    config.get_asset_factory_config().get_platform_factory()
        .create_asset(args.properties);
#ifdef DEBUG
    std::cout << "<- GetAssetHandler::handle" << std::endl;
#endif
}