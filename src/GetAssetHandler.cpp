#include "events/handlers/GetAssetHandler.h"
#include "events/handlers/CreatePlatformHandler.h"
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
    SPlat::Event event;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(event);
    }

    // send to server as request
    Request request = {
        .content_type=Request::ContentType::Event,
        .body=serialized
    };
    ControllerInterface& ctl = entrypoint.get_controller();
    Response response = ctl.await(request);

    if (response.status != 200) {
        std::cerr << "Could not retrieve asset (" << response.status << "):" << std::endl;
        std::cerr << response.body << std::endl;
        throw std::logic_error("");  // TODO create some TCPException class
    }
    
    Model::AssetProperties asset_properties;
    {
        std::stringstream iss; iss << response.body;
        cereal::JSONInputArchive iar(iss);
        iar(asset_properties);
    }

    config.get_asset_factory_config().get_platform_factory()
        .create_asset(asset_properties);
#ifdef DEBUG
    std::cout << "<- GetAssetHandler::handle" << std::endl;
#endif
}