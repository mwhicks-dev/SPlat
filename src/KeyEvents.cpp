#include "KeyEvents.h"

std::set<sf::Keyboard::Key> SPlat::KeyEvent::held;
std::mutex SPlat::KeyEvent::held_lock;

std::string SPlat::KeyPressEvent::KEY_PRESS_EVENT_TAG = "key_press";

std::string SPlat::KeyReleaseEvent::KEY_RELEASE_EVENT_TAG = "key_release";

/// @brief function executed on key press event
/// @param serialized arguments passed as serialized string
void SPlat::KeyPressEvent::handler(std::string serialized) {
    // deserialize KeyEventArgs from args
    KeyEventArgs args;
    std::stringstream ss; ss << serialized;
    {
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // Set pressed key as held
    SPlat::KeyEvent::held_lock.lock();
    if (SPlat::KeyEvent::held.count(args.key) == 0)
        SPlat::KeyEvent::held.insert(args.key);
    SPlat::KeyEvent::held_lock.unlock();
}

/// @brief function executed on key release event
/// @param serialized arguments passed as serialized string
void SPlat::KeyReleaseEvent::handler(std::string serialized) {
    // deserialize KeyEventArgs from args
    KeyEventArgs args;
    std::stringstream ss; ss << serialized;
    {
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // Unset pressed key as held
    SPlat::KeyEvent::held_lock.lock();
    if (SPlat::KeyEvent::held.count(args.key) > 0)
        SPlat::KeyEvent::held.erase(args.key);
    SPlat::KeyEvent::held_lock.unlock();
}
