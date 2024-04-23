#include "events/KeyEvents.h"
#include "events/Command.h"
#include "events/Listener.h"

#include <cereal/archives/json.hpp>

#include <sstream>

using namespace SPlat::Events;

KeyEvent::KeyEvent(sf::Keyboard::Key key) {
    this->key = key;
}

void KeyEvent::raise() {
    // serialize args to JSON string
    Args args = {.key=key};
    std::stringstream ss;
    {
        cereal::JSONOutputArchive oar(ss);
        oar(args);
    }

    // create new command
    Command cmd = {
        .priority=-1,
        .type=get_type(),
        .args=ss.str()
    };

    // send to foreground listener
    ForegroundListener::get_instance().push_command(cmd);
}

void KeyPressEvent::handler(std::string serialized) {
    // deserialize args from JSON string
    Args args;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // add keyboard key to pressed keys
    keys_held_lock.lock();
    keys_held.insert(args.key);
    keys_held_lock.unlock();
}

void KeyReleaseEvent::handler(std::string serialized) {
    // deserialize args from JSON string
    Args args;
    {
        std::stringstream ss; ss << serialized;
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // remove keyboard key from pressed keys
    keys_held_lock.lock();
    keys_held.erase(args.key);
    keys_held_lock.unlock();
}
