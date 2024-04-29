#include "Client.h"
#include "Config.h"
#include "model/Character.h"
#include "events/AssetEvents.h"
#include "events/CharacterEvents.h"
#include "events/MovingPlatformEvents.h"
#include "events/PlatformEvents.h"
#include "events/KeyEvents.h"
#include "events/Listener.h"

#include <cereal/archives/json.hpp>

using namespace SPlat;

/// @brief overriden keypress event handler with movement
/// @param serialized serialized KeyEventArgs
static void keypress_override(std::string serialized) {
    // set key held
    Events::KeyPressEvent::handler(serialized);

    try {
        // get asset
        size_t id = Events::ControlAssetEvent::get_controlled_asset_id();
        Model::Character& ctl = (Model::Character&) Config::get_instance()
            .get_asset_factory_config().get_character_factory().read_asset(id);
        
        // deserialize KeyEventArgs from args
        Events::KeyEvent::Args args;
        std::stringstream ss; ss << serialized;
        {
            cereal::JSONInputArchive iar(ss);
            iar(args);
        }

        // update velocity based on key pressed
        if (args.key == sf::Keyboard::Key::Left) {
            ctl.get_moving_properties().set_velocity(ctl
                .get_moving_properties().get_velocity() + sf::Vector2f(-300, 0));
        } else if (args.key == sf::Keyboard::Key::Right) {
            ctl.get_moving_properties().set_velocity(ctl
                .get_moving_properties().get_velocity() + sf::Vector2f(300, 0));
        } else if (args.key == sf::Keyboard::Key::Up 
                && ((Model::CharacterProperties&) ctl.get_moving_properties())
                .get_standing_on() != nullptr) {
            ctl.get_moving_properties().set_velocity(ctl
                .get_moving_properties().get_velocity() + sf::Vector2f(-490, 0));
        } else if (args.key == sf::Keyboard::Escape) {
            if (Config::get_instance().get_timing_config().get_display_timeline().get_paused())
                Config::get_instance().get_timing_config().get_display_timeline().unpause();
            else Config::get_instance().get_timing_config().get_display_timeline().pause();
        }

    } catch (std::logic_error & e) {
        std::cout << e.what() << std::endl;
    }
}

/// @brief overriden keyrelease event handler with movement
/// @param serialized serialized KeyEventArgs
static void keyrelease_override(std::string serialized) {
    // use default handler to unset key held
    Events::KeyReleaseEvent::handler(serialized);

    try {
        // get asset
        size_t id = Events::ControlAssetEvent::get_controlled_asset_id();
        Model::Character& ctl = (Model::Character&) Config::get_instance()
            .get_asset_factory_config().get_character_factory().read_asset(id);
        
        // deserialize KeyEventArgs from args
        Events::KeyEvent::Args args;
        std::stringstream ss; ss << serialized;
        {
            cereal::JSONInputArchive iar(ss);
            iar(args);
        }

        // update velocity based on key pressed
        if (args.key == sf::Keyboard::Key::Left) {
            ctl.get_moving_properties().set_velocity(ctl
                .get_moving_properties().get_velocity() + sf::Vector2f(300, 0));
        } else if (args.key == sf::Keyboard::Key::Right) {
            ctl.get_moving_properties().set_velocity(ctl
                .get_moving_properties().get_velocity() + sf::Vector2f(-300, 0));
        }

    } catch (std::logic_error & e) {
        std::cout << e.what() << std::endl;
    }
}

int main() {
    // This should be done first -- framerate limit sleeps for 1s
    Client client;
    
    // Create assets with events
    Model::CharacterProperties character_properties(
        sf::Vector2f(100, 100),  // position
        sf::Vector2f(50, 100),  // size
        sf::Color(0, 0, 0, 255),  // fill_color
        sf::Vector2f(0, 0),  // velocity
        Config::get_instance().get_timing_config().get_display_timeline().get_time(),  // last_updated
        nullptr  // standing_on
    );
    Model::Character& character = (Model::Character&) Config::get_instance()
        .get_asset_factory_config().get_character_factory().create_asset(character_properties);

    Events::ForegroundListener::get_instance().set_handler(Events::KeyPressEvent::get_type(), keypress_override);
    Events::ForegroundListener::get_instance().set_handler(Events::KeyReleaseEvent::get_type(), keyrelease_override);
    client.start();
}
