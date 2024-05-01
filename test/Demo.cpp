#include "Client.h"
#include "ConfigInterface.h"
#include "model/Character.h"
#include "model/Platform.h"
#include "model/MovingPlatform.h"
#include "events/KeyPressCommandHandler.h"
#include "events/KeyReleaseCommandHandler.h"
#include "events/CreatePlatformHandler.h"
#include "events/CreateMovingPlatformHandler.h"
#include "events/CreateCharacterHandler.h"
#include "FauxServerController.h"

#include <cereal/archives/json.hpp>

#include <thread>

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
    
    cli.get_foreground_listener().set_handler(Events::KeyPressCommandHandler::get_event_type(), *new KeyPressOverride());
    cli.get_foreground_listener().set_handler(Events::KeyReleaseCommandHandler::get_event_type(), *new KeyReleaseOverride());

    cli.get_config().get_timing_config().update_framerate_limit(90);

    // Create assets
    {
        Model::AssetProperties properties(
            sf::Vector2f(100, 100),  // position
            sf::Vector2f(50, 100),  // size
            sf::Color::Magenta  // fill_color
        );
        Events::CreateCharacterHandler::Args args = {
            .properties=properties
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
        cli.get_background_listener().push_command(cmd);
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
        cli.get_background_listener().push_command(cmd);
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
        cli.get_background_listener().push_command(cmd);
        std::unordered_set<size_t> ids = cli.get_object_model().get_ids(); size_t max = 0;
        Model::MovingPlatform * moving_platform = nullptr;
        size_t client_id = conf.get_environment().get_entrypoint_id();
        for (size_t id : ids) {
            std::cout << id << std::endl;
            try {
                Model::MovingPlatform& mplat 
                    = dynamic_cast<Model::MovingPlatform&>(cli
                    .get_object_model().read_asset(id));
            } catch (std::bad_cast&) {}
        }

        // add states
        /*Model::MovingPlatformProperties& moving_platform_properties = moving_platform->get_moving_platform_properties();
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
        moving_platform_properties.set_states(states);*/
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    cli.start();
}
