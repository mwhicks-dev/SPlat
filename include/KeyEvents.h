#ifndef SPLAT_KEYEVENTS_H
#define SPLAT_KEYEVENTS_H

#include <set>
#include <mutex>

#include <SFML/Window/Keyboard.hpp>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

#include "Event.h"

namespace SPlat {

    /// @brief class holding shared (static) data for key-related events
    class KeyEvent : public Event {

        /// @brief set of keys that are currently held
        static std::set<sf::Keyboard::Key> held;

        /// @brief mutex required to access held
        static std::mutex held_lock;

    public:

        /// @brief static function checking if key is held
        /// @param key keybaord key to check
        static is_key_held(sf::Keyboard::Key key) {
            // check if keyboard is in held set
            held_lock.lock();
            bool out = held.count(key) > 0;
            held_lock.unlock();

            return out;
        }

    };

    /// @brief event of pressing a keyboard key
    class KeyPressEvent : public KeyEvent {

    public:

        /// @brief unique identifier for key press events
        static const std::string KEY_PRESS_EVENT_TAG = "key_press";

        /// @brief convert keypress to serializable event
        /// @param key keyboard key pressed
        KeyPressEvent(sf::Keyboard::Key key) {
            // Serialize key to JSON string
            KeyEventArgs args = {key};
            std::stringstream ss;
            {
                cereal::JSONOutputArchive oar(ss);
                oar(args);
            }

            // Construct event with appropriate type/args
            Event(KEY_PRESS_EVENT_TAG, ss.str());
        }

    };

    /// @brief event of releasing a keyboard key
    class KeyReleaseEvent : public KeyEvent {

    public:

        /// @brief unique identifier for key release events
        static const std::string KEY_RELEASE_EVENT_TAG = "key_release";

        /// @brief convert keyrelease to serializable event
        /// @param key keyboard key released
        KeyReleaseEvent(sf::Keyboard::Key key) {
            // Serialize key to JSON string
            KeyEventArgs args = {key};
            std::stringstream ss;
            {
                cereal::JSONOutputArchive oar(ss);
                ss(args);
            }

            // Construct event with appropriate type/args
            Event(KEY_RELEASE_EVENT_TAG, ss.str());
        }

    }

    /// @brief struct just containing pressed/released key
    struct KeyEventArgs {
        
        /// @brief pressed or released key
        sf::Keyboard::Key key;

        /// @brief formulation for converting these args to serializable form
        /// @param ar archive class used to serialize args
        template <class Archive>
        void serialize(Archive &ar) {
            ar(key);
        }
    };

}

#endif
