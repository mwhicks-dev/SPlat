#include "Client.h"
#include "ConfigInterface.h"
#include "model/Character.h"
#include "model/Platform.h"
#include "model/MovingPlatform.h"
#include "events/KeyPressHandler.h"
#include "events/KeyReleaseHandler.h"
#include "events/CreatePlatformHandler.h"
#include "events/CreateMovingPlatformHandler.h"
#include "events/CreateCharacterHandler.h"
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

    cli.get_config().get_timing_config().update_framerate_limit(90);

    // Create assets
    {
        Model::AssetProperties properties(
            sf::Vector2f(100, 100),  // position
            sf::Vector2f(50, 100),  // size
            sf::Color::Magenta  // fill_color
        );
        Events::CreateCharacterHandler::Args args = {
            .properties=properties,
            .set_controlled=true
        };
        std::stringstream ss;
        {
            cereal::JSONOutputArchive oar(ss);
            oar(args);
        }
        Events::Command cmd = {
            .priority=-1,
            .type=Events::CreateCharacterHandler::get_type(),
            .args=ss.str()
        };
        Event event = {
            .event_time=0,
            .command=cmd,
            .client_side=false,
            .sender=1,
        };
        cli.get_background_listener().push_event(event);
    }
    {
        Model::AssetProperties properties(
            sf::Vector2f(0, 500),  // position
            sf::Vector2f(400, 100),  // size
            sf::Color::Green  // fill_color
        );
        Events::CreatePlatformHandler::Args args = {
            .properties=properties
        };
        std::stringstream ss;
        {
            cereal::JSONOutputArchive oar(ss);
            oar(args);
        }
        Events::Command cmd = {
            .priority=-1,
            .type=Events::CreatePlatformHandler::get_type(),
            .args=ss.str()
        };
        Event event = {
            .event_time=0,
            .command=cmd,
            .client_side=false,
            .sender=1,
        };
        cli.get_background_listener().push_event(event);
    }
    {
        Model::AssetProperties properties(
            sf::Vector2f(200, 300),  // position
            sf::Vector2f(150, 25),  // size
            sf::Color::White  // fill_color
        );
        Events::CreateMovingPlatformHandler::Args args = {
            .properties=properties
        };

        // add states
        std::vector<Model::State> states;
        states.push_back(
            Model::State(
                sf::Vector2f(5, 0) * conf.get_environment().get_unit(),  // velocity
                1.5 * conf.get_timing_config().get_anchor_steps_per_second(),  // anchor_steps
                true  // repeat
            )
        );
        states.push_back(
            Model::State(
                sf::Vector2f(-15, 0) * conf.get_environment().get_unit(),  // velocity
                0.5 * conf.get_timing_config().get_anchor_steps_per_second(),  // anchor_steps
                true  // repeat
            )
        );
        args.states = states;
        std::stringstream ss;
        {
            cereal::JSONOutputArchive oar(ss);
            oar(args);
        }
        Events::Command cmd = {
            .priority=-1,
            .type=Events::CreateMovingPlatformHandler::get_type(),
            .args=ss.str()
        };
        Event event = {
            .event_time=0,
            .command=cmd,
            .client_side=false,
            .sender=1,
        };
        cli.get_background_listener().push_event(event);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    cli.start();
}
