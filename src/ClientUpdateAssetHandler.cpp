#include "events/handlers/ClientReadAssetHandler.h"
#include "events/handlers/ClientUpdateAssetHandler.h"
#include "ControllerInterface.h"
#include "Entrypoint.h"
#include "Event.h"

#include <cereal/archives/json.hpp>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Events;

void ClientUpdateAssetHandler::handle(std::string serialized) {
#ifdef DEBUG
    std::cout << "-> ClientUpdateAssetHandler::handle(" << serialized << ")" << std::endl;
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

    ControllerInterface& ctl = entrypoint.get_controller();
    if (event.sender == environment.get_entrypoint_id()) {
        // send to server as request
        Request request = {
            .content_type=Request::ContentType::Event,
            .body=serialized
        }; 
        
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
    } else {
        try {
            // get asset from GOM
            Model::Asset& asset = entrypoint.get_object_model().read_asset(args.id);
            Model::AssetProperties& asset_properties = asset.get_asset_properties();
            asset_properties.set_fill_color(args.properties.get_fill_color());
            asset_properties.set_position(args.properties.get_position());
            asset_properties.set_size(args.properties.get_size());
        } catch (std::exception&) {
            // retrieve asset from server if cannot find
            ClientReadAssetHandler::Args get_asset_args = {
                .id=args.id
            };
            std::stringstream get_asset_args_ss;
            {
                cereal::JSONOutputArchive oar(get_asset_args_ss);
                oar(get_asset_args);
            }
            Command get_asset_command = {
                .priority=-2,
                .type=ClientReadAssetHandler::get_type(),
                .args=get_asset_args_ss.str()
            };
            Event get_asset_event = {
                .command=get_asset_command,
                .sender=environment.get_entrypoint_id()
            };
            std::stringstream get_asset_event_ss;
            {
                cereal::JSONOutputArchive oar(get_asset_event_ss);
                oar(get_asset_event);
            }
            Request get_asset_request = {
                .content_type=Request::ContentType::Event,
                .body=get_asset_event_ss.str()
            };
            Response get_asset_response = ctl.await(get_asset_request);

            if (get_asset_response.status != 200) {
                std::cerr << "Could not retrieve asset " << args.id << "(";
                std::cerr << get_asset_response.status << "):" << std::endl;
                std::cerr << get_asset_response.body << std::endl;
                throw std::logic_error("");  // TODO create some TCPException class
            }

            Model::AssetProperties asset_properties;
            {
                std::stringstream asset_properties_ss;
                asset_properties_ss << get_asset_response.body;
                cereal::JSONInputArchive iar(asset_properties_ss);
                iar(asset_properties);
            }

            // TODO: find better check for asset type than assuming they're a platform
            config.get_asset_factory_config().get_platform_factory()
                .create_asset(asset_properties);
        }
    }
    
#ifdef DEBUG
    std::cout << "<- ClientUpdateAssetHandler::handle" << std::endl;
#endif
}
