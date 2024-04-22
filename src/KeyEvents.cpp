#include "events/KeyEvents.h"

using namespace SPlat::Events;

std::set<sf::Keyboard::Key> KeyEvent::held;
std::mutex KeyEvent::held_lock;

std::string KeyPressEvent::TYPE = "key_press";

std::string KeyReleaseEvent::TYPE = "key_release";

/// @brief function executed on key press event
/// @param serialized arguments passed as serialized string
void KeyPressEvent::handler(std::string serialized) {
    // deserialize KeyEventArgs from args
    KeyEventArgs args;
    std::stringstream ss; ss << serialized;
    {
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // Set pressed key as held
    KeyEvent::held_lock.lock();
    if (KeyEvent::held.count(args.key) == 0)
        KeyEvent::held.insert(args.key);
    KeyEvent::held_lock.unlock();
}

/// @brief function executed on key release event
/// @param serialized arguments passed as serialized string
void KeyReleaseEvent::handler(std::string serialized) {
    // deserialize KeyEventArgs from args
    KeyEventArgs args;
    std::stringstream ss; ss << serialized;
    {
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // Unset pressed key as held
    KeyEvent::held_lock.lock();
    if (KeyEvent::held.count(args.key) > 0)
        KeyEvent::held.erase(args.key);
    KeyEvent::held_lock.unlock();
}
