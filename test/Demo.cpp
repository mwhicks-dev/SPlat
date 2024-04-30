#include "Client.h"
#include "ConfigInterface.h"
#include "model/Character.h"
#include "model/Platform.h"
#include "model/MovingPlatform.h"
#include "events/KeyPressCommandHandler.h"
#include "events/KeyReleaseCommandHandler.h"

#include <cereal/archives/json.hpp>

using namespace SPlat;

class KeyPressOverride : public SPlat::Events::KeyPressCommandHandler {

public:

    void handle(std::string serialized) override {
        Args args;
        {
            std::stringstream ss; ss << serialized;
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

class KeyReleaseOverride : public Events::KeyReleaseCommandHandler {

public:

    void handle(std::string serialized) override {
        Args args;
        {
            std::stringstream ss; ss << serialized;
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
    // Create assets
    {
        Model::AssetProperties properties(
            sf::Vector2f(100, 100),  // position
            sf::Vector2f(50, 100),  // size
            sf::Color::Magenta  // fill_color
        );
        Model::Asset& asset = conf.get_asset_factory_config().get_character_factory().create_asset(properties);
        Model::Character& character = dynamic_cast<Model::Character&>(asset);
        Client::get_instance().get_config().get_environment().set_controlled_asset(&character);
    }
    {
        Model::AssetProperties properties(
            sf::Vector2f(0, 500),  // position
            sf::Vector2f(400, 100),  // size
            sf::Color::Green  // fill_color
        );
        conf.get_asset_factory_config().get_platform_factory().create_asset(properties);
    }
    {
        Model::AssetProperties properties(
            sf::Vector2f(200, 300),  // position
            sf::Vector2f(150, 25),  // size
            sf::Color::White  // fill_color
        );
        Model::Asset& asset = conf.get_asset_factory_config().get_moving_platform_factory().create_asset(properties);
        Model::MovingPlatform& moving_platform = dynamic_cast<Model::MovingPlatform&>(asset);

        // add states
        Model::MovingPlatformProperties& moving_platform_properties = moving_platform.get_moving_platform_properties();
        std::vector<Model::State> states = moving_platform_properties.get_states();
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
        moving_platform_properties.set_last_state_change(conf.get_timing_config().get_anchor_timeline().get_time());
        moving_platform_properties.set_states(states);
    }
    
    conf.get_environment().set_framerate_limit(90);

    cli.get_foreground_listener().set_handler(Events::KeyPressCommandHandler::get_event_type(), *new KeyPressOverride());
    cli.get_foreground_listener().set_handler(Events::KeyReleaseCommandHandler::get_event_type(), *new KeyReleaseOverride());

    cli.start();
}
