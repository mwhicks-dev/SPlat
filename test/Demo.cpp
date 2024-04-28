#include "Client.h"
#include "Runtime.h"
#include "model/AssetFactory.h"
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
        Model::Character ctl = Model::AssetFactory<Model::Character>::read_asset(id);
        
        // deserialize KeyEventArgs from args
        Events::KeyEvent::Args args;
        std::stringstream ss; ss << serialized;
        {
            cereal::JSONInputArchive iar(ss);
            iar(args);
        }

        // update velocity based on key pressed
        if (args.key == sf::Keyboard::Key::Left) {
            Events::AddVelocityEvent event(ctl.id, sf::Vector2f(-300, 0));
            event.raise();
        } else if (args.key == sf::Keyboard::Key::Right) {
            Events::AddVelocityEvent event(ctl.id, sf::Vector2f(300, 0));
            event.raise();
        } else if (args.key == sf::Keyboard::Key::Up 
                && ctl.standing_on != nullptr) {
            Events::AddVelocityEvent event(ctl.id, sf::Vector2f(0, -490));
            event.raise();
        } else if (args.key == sf::Keyboard::Escape) {
            if (Runtime::get_instance().get_display_timeline().get_paused())
                Runtime::get_instance().get_display_timeline().unpause();
            else Runtime::get_instance().get_display_timeline().pause();
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
        Model::Character ctl = Model::AssetFactory<Model::Character>::read_asset(id);
        
        // deserialize KeyEventArgs from args
        Events::KeyEvent::Args args;
        std::stringstream ss; ss << serialized;
        {
            cereal::JSONInputArchive iar(ss);
            iar(args);
        }

        // update velocity based on key pressed
        if (args.key == sf::Keyboard::Key::Left) {
            Events::AddVelocityEvent event(ctl.id, sf::Vector2f(300, 0));
            event.raise();
        } else if (args.key == sf::Keyboard::Key::Right) {
            Events::AddVelocityEvent event(ctl.id, sf::Vector2f(-300, 0));
            event.raise();
        }

    } catch (std::logic_error & e) {
        std::cout << e.what() << std::endl;
    }
}

int main() {
    // Create assets with events
    SPlat::Events::CreateControlCharacterEvent({
        sf::Vector2f(100, 100), // position
        sf::Vector2f(50, 100),  // size
        SPlat::Model::Character::TYPE  // type
    }).raise();

    SPlat::Events::CreatePlatformEvent({
        sf::Vector2f(0, 500),
        sf::Vector2f(400, 200),
        SPlat::Model::Platform::TYPE
    }).raise();

    SPlat::Events::CreateMovingPlatformEvent(
        {  // AssetProperties
            .position=sf::Vector2f(300, 100),
            .size=sf::Vector2f(200, 25),
            .type=SPlat::Model::MovingPlatform::TYPE
        }, {  // std::vector<State>
            {
                .position=sf::Vector2f(200, 300),
                .ticks_til_next=100
            }, {
                .position=sf::Vector2f(400, 300),
                .ticks_til_next=100
            }
        }
    ).raise();

    Events::ForegroundListener::get_instance().set_handler(Events::KeyPressEvent::get_type(), keypress_override);
    Events::ForegroundListener::get_instance().set_handler(Events::KeyReleaseEvent::get_type(), keyrelease_override);
    
    Client client;
    client.set_framerate_limit(60);
    client.start();
}
