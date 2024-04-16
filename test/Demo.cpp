#include "Client.h"
#include "model/Character.h"
#include "utilities/Functions.h"

using namespace SPlat;

/// @brief overriden keypress event handler with movement
/// @param serialized serialized KeyEventArgs
static void keypress_override(std::string serialized) {
    // set key held
    Events::KeyPressEvent::handler(serialized);

    try {
        // get asset
        size_t id = Events::ControlAssetEvent::get_controlled_asset_id();
        Model::Asset& ctl = Model::GameObjectModel
            ::get_instance().read_asset(id);
        Model::AssetProperties ctl_properties = ctl.get_properties();
        
        // deserialize KeyEventArgs from args
        Events::KeyEventArgs args;
        std::stringstream ss; ss << serialized;
        {
            cereal::JSONInputArchive iar(ss);
            iar(args);
        }

        // update velocity based on key pressed
        if (args.key == sf::Keyboard::Key::Left) {
            Events::UpdateAssetEvent event(
                ctl.id, ctl_properties, sf::Vector2f(-15, 0));
            event.priority = -1;
            event.raise();
        } else if (args.key == sf::Keyboard::Key::Right) {
            Events::UpdateAssetEvent event(
                ctl.id, ctl_properties, sf::Vector2f(15, 0));
            event.priority = -1;
            event.raise();
        } else if (args.key == sf::Keyboard::Key::Up 
                && ctl.standing_on != nullptr) {
            Events::UpdateAssetEvent event(
                ctl.id, ctl_properties, sf::Vector2f(0, -25));
            event.priority = -1;
            event.raise();
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
        Model::Asset& ctl = Model::GameObjectModel
            ::get_instance().read_asset(id);
        Model::AssetProperties ctl_properties = ctl.get_properties();
        
        // deserialize KeyEventArgs from args
        Events::KeyEventArgs args;
        std::stringstream ss; ss << serialized;
        {
            cereal::JSONInputArchive iar(ss);
            iar(args);
        }

        // update velocity based on key pressed
        if (args.key == sf::Keyboard::Key::Left) {
            Events::UpdateAssetEvent event(
                ctl.id, ctl_properties, sf::Vector2f(15, 0));
            event.priority = -1;
            event.raise();
        } else if (args.key == sf::Keyboard::Key::Right) {
            Events::UpdateAssetEvent event(
                ctl.id, ctl_properties, sf::Vector2f(-15, 0));
            event.priority = -1;
            event.raise();
        }

    } catch (std::logic_error & e) {/* OK */}
}

int main() {
    // Create assets with events
    SPlat::Events::Event event;
    event = SPlat::Events::CreateControlAssetEvent({
        sf::Vector2f(100, 100), // position
        sf::Vector2f(50, 100),  // size
        SPlat::Model::Character::TYPE  // type
    });
    event.raise();

    event = SPlat::Events::CreateAssetEvent({
        sf::Vector2f(0, 500),
        sf::Vector2f(400, 200),
        SPlat::Model::Platform::TYPE
    });
    event.raise();

    Utilities::set_default_handlers();

    Events::Event::handlers[Events::KeyPressEvent::TYPE] =
        keypress_override;
    Events::Event::handlers[Events::KeyReleaseEvent::TYPE] =
        keyrelease_override;

    Client client; client.start();
}
