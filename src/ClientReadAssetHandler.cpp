#include "events/handlers/ClientReadAssetHandler.h"
#include "ControllerInterface.h"
#include "Entrypoint.h"
#include "Event.h"

#include <cereal/archives/json.hpp>

using namespace SPlat::Events;

void ClientReadAssetHandler::handle(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> ClientReadAssetHandler::handle(" << serialized << ")" << std::endl;
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

    int collision_priority = asset_properties.get_collision_priority();
    AssetFactoryConfigInterface& afc = config.get_asset_factory_config();
    switch (collision_priority) {
    case 0:
        afc.get_moving_platform_factory().create_asset(asset_properties);
        break;
    case -1:
        afc.get_character_factory().create_asset(asset_properties);
        break;
    default:  // do platform if cannot recognize
        afc.get_platform_factory().create_asset(asset_properties);
        break;
    }
#ifdef DEBUG
    std::cout << "<- ClientReadAssetHandler::handle" << std::endl;
#endif
}