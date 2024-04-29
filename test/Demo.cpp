#include "Client.h"
#include "Config.h"
#include "model/Character.h"
#include "model/Platform.h"
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

    // deserialize KeyEventArgs from args
    Events::KeyEvent::Args args;
    std::stringstream ss; ss << serialized;
    {
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    if (args.key == sf::Keyboard::Key::Escape) {
        LocalTimeline& dtl = Config::get_instance().get_timing_config().get_display_timeline();
        if (dtl.get_paused()) {
            dtl.unpause();
        } else {
            dtl.pause();
        }
    }

    try {
        // get asset
        Model::Character* ctl = Config::get_instance()
            .get_environment().get_controlled_asset();
        if (ctl == nullptr) {
#ifdef DEBUG
            std::cout << "No asset set as controlled" << std::endl;
#endif
            return;
        }

        // update velocity based on key pressed
        if (args.key == sf::Keyboard::Key::Left) {
            ctl->get_moving_properties().set_velocity(ctl
                ->get_moving_properties().get_velocity() + sf::Vector2f(
                    Config::get_instance().get_environment().get_unit() * -15, 0));
        } else if (args.key == sf::Keyboard::Key::Right) {
            ctl->get_moving_properties().set_velocity(ctl
                ->get_moving_properties().get_velocity() + sf::Vector2f(
                    Config::get_instance().get_environment().get_unit() * 15, 0));
        } else if (args.key == sf::Keyboard::Key::Up 
                && ((Model::CharacterProperties&) ctl->get_moving_properties())
                .get_standing_on() != nullptr) {
            ctl->get_moving_properties().set_velocity(ctl
                ->get_moving_properties().get_velocity() + sf::Vector2f(
                    0, Config::get_instance().get_environment().get_unit() * -75));
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

    // deserialize KeyEventArgs from args
    Events::KeyEvent::Args args;
    std::stringstream ss; ss << serialized;
    {
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    try {
        // get asset
        Model::Character* ctl = Config::get_instance()
            .get_environment().get_controlled_asset();
        if (ctl == nullptr) {
#ifdef DEBUG
            std::cout << "No asset set as controlled" << std::endl;
#endif
            return;
        }

        // update velocity based on key pressed
        if (args.key == sf::Keyboard::Key::Left) {
            ctl->get_moving_properties().set_velocity(ctl
                ->get_moving_properties().get_velocity() - sf::Vector2f(
                    Config::get_instance().get_environment().get_unit() * -15, 0));
        } else if (args.key == sf::Keyboard::Key::Right) {
            ctl->get_moving_properties().set_velocity(ctl
                ->get_moving_properties().get_velocity() - sf::Vector2f(
                    Config::get_instance().get_environment().get_unit() * 15, 0));
        }

    } catch (std::logic_error & e) {
        std::cout << e.what() << std::endl;
    }
}

int main() {
    // Create assets with events
    Model::CharacterProperties character_properties(
        sf::Vector2f(100, 100),  // position
        sf::Vector2f(50, 100),  // size
        sf::Color(255, 255, 255, 255),  // fill_color
        sf::Vector2f(0, 0),  // velocity
        Config::get_instance().get_timing_config().get_display_timeline().get_time(),  // last_updated
        nullptr  // standing_on
    );
    Model::Character& character = (Model::Character&) Config::get_instance()
        .get_asset_factory_config().get_character_factory().create_asset(character_properties);
    Config::get_instance().get_environment().set_controlled_asset(&character);

    Model::AssetProperties platform_properties(
        sf::Vector2f(0, 500),  // position
        sf::Vector2f(400, 100),  // size
        sf::Color(255, 255, 255, 255),  // fill color
        -2
    );
    Model::Platform& platform = (Model::Platform&) Config::get_instance()
        .get_asset_factory_config().get_platform_factory().create_asset(platform_properties);

    Events::ForegroundListener::get_instance().set_handler(Events::KeyPressEvent::get_type(), keypress_override);
    Events::ForegroundListener::get_instance().set_handler(Events::KeyReleaseEvent::get_type(), keyrelease_override);
    
    Client client; client.start();
}
