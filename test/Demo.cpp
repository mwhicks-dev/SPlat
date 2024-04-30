#include "Client.h"
#include "ConfigInterface.h"
#include "model/Character.h"
#include "model/Platform.h"
#include "model/MovingPlatform.h"

#include <cereal/archives/json.hpp>

using namespace SPlat;

int main() {
    ConfigInterface& conf = Client::get_instance().get_config();
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

    Client::get_instance().start();
}
