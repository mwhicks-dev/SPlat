#include "Client.h"
#include "ConfigInterface.h"
#include "model/Character.h"
#include "model/Platform.h"
#include "model/MovingPlatform.h"
#include "events/handlers/KeyPressHandler.h"
#include "events/handlers/KeyReleaseHandler.h"
#include "events/handlers/ClientCreatePlatformHandler.h"
#include "events/handlers/ClientCreateMovingPlatformHandler.h"
#include "events/handlers/ClientCreateCharacterHandler.h"
#include "FauxServerController.h"

#include <cereal/archives/json.hpp>

#include <thread>

using namespace SPlat;

class KeyPressOverride : public SPlat::Events::KeyPressHandler {

public:

    void handle(std::string serialized) override {
        Event event;
        {
            std::stringstream ss; ss << serialized;
            cereal::JSONInputArchive iar(ss);
            iar(event);
        }

        Args args;
        {
            std::stringstream ss; ss << event.command.args;
            cereal::JSONInputArchive iar(ss);
            iar(args);
        }

        if (!args.key == sf::Keyboard::Key::Left 
                && !args.key == sf::Keyboard::Key::Right
                && !args.key == sf::Keyboard::Key::Up
                && !args.key == sf::Keyboard::Key::Escape) return;
        
        ConfigInterface& conf = Client::get_instance().get_config();

        if (conf.get_environment().get_held_keys().count(args.key) > 0) return;
        conf.get_environment().add_held_key(args.key);

        if (args.key == sf::Keyboard::Escape)
            return conf.get_timing_config().get_display_timeline().pause();

        Model::Character* controlled = conf.get_environment().get_controlled_asset();
        if (controlled == nullptr) return;

        Model::MovingProperties& moving_properties = controlled->get_moving_properties();

        if (args.key == sf::Keyboard::Left) {
            // move left
            moving_properties.set_velocity(moving_properties.get_velocity() 
                + sf::Vector2f(conf.get_environment().get_unit() * -10, 0));
        } else if (args.key == sf::Keyboard::Right) {
            // move right
            moving_properties.set_velocity(moving_properties.get_velocity() 
                + sf::Vector2f(conf.get_environment().get_unit() * 10, 0));
        } else if (args.key == sf::Keyboard::Up 
                && controlled->get_character_properties().get_standing_on() != nullptr) {
            // jump
            moving_properties.set_velocity(moving_properties.get_velocity() 
                + sf::Vector2f(0, conf.get_environment().get_unit() * -11.5));
        }
    }

};

class KeyReleaseOverride : public Events::KeyReleaseHandler {

public:

    void handle(std::string serialized) override {
        Event event;
        {
            std::stringstream ss; ss << serialized;
            cereal::JSONInputArchive iar(ss);
            iar(event);
        }

        Args args;
        {
            std::stringstream ss; ss << event.command.args;
            cereal::JSONInputArchive iar(ss);
            iar(args);
        }

        if (!args.key == sf::Keyboard::Key::Left 
                && !args.key == sf::Keyboard::Key::Right) return;
        
        ConfigInterface& conf = Client::get_instance().get_config();

        if (conf.get_environment().get_held_keys().count(args.key) == 0) return;
        conf.get_environment().remove_held_key(args.key);

        Model::Character* controlled = conf.get_environment().get_controlled_asset();
        if (controlled == nullptr) return;
        
        Model::MovingProperties& moving_properties = controlled->get_moving_properties();

        if (args.key == sf::Keyboard::Left) {
            // move left
            moving_properties.set_velocity(moving_properties.get_velocity() 
                - sf::Vector2f(conf.get_environment().get_unit() * -10, 0));
        } else if (args.key == sf::Keyboard::Right) {
            // move right
            moving_properties.set_velocity(moving_properties.get_velocity() 
                - sf::Vector2f(conf.get_environment().get_unit() * 10, 0));
        }
    }

};

int main() {
    Client& cli = Client::get_instance();
    ConfigInterface& conf = cli.get_config();
    
    cli.get_foreground_listener().set_handler(Events::KeyPressHandler::get_event_type(), *new KeyPressOverride());
    cli.get_foreground_listener().set_handler(Events::KeyReleaseHandler::get_event_type(), *new KeyReleaseOverride());

    cli.get_config().get_environment().set_req_rep_address("tcp://localhost:5555");
    std::thread t(&Client::start, &cli);

    while (cli.get_config().get_environment().get_entrypoint_id() == 0) {}

    {
        Model::AssetProperties properties(
            sf::Vector2f(100, 100),  // position
            sf::Vector2f(50, 100),  // size
            sf::Color::Magenta  // fill_color
        );
        properties.set_owner(cli.get_config().get_environment().get_entrypoint_id());
        Events::ClientCreateCharacterHandler::Args args = {
            .properties=properties,
            .set_controlled=true
        };
        std::stringstream ss;
        {
            cereal::JSONOutputArchive oar(ss);
            oar(args);
        }
        std::cout << ss.str() << std::endl;
        Events::Command cmd = {
            .priority=-1,
            .type=Events::ClientCreateCharacterHandler::get_type(),
            .args=ss.str(),
        };
        Event event = {
            .command=cmd,
            .sender=cli.get_config().get_environment().get_entrypoint_id()
        };
        std::stringstream event_ss;
        {
            cereal::JSONOutputArchive oar(event_ss);
            oar(event);
        }
        Request request = Request {
            .content_type=Request::ContentType::Event,
            .body=event_ss.str()
        };

        cli.get_background_listener().push_event(event);
    }
    
    t.join();
}
