#ifndef SPLAT_KEYEVENTS_H
#define SPLAT_KEYEVENTS_H

#include <set>
#include <mutex>

#include <SFML/Window/Keyboard.hpp>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

#include "Event.h"

namespace SPlat {

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

    static void key_press_handler(std::string);

    static void key_release_handler(std::string);

    /// @brief class holding shared (static) data for key-related events
    class KeyEvent : public Event {

    public:

        /// @brief set of keys that are currently held
        static std::set<sf::Keyboard::Key> held;

        /// @brief mutex required to access held
        static std::mutex held_lock;

        /// @brief static function checking if key is held
        /// @param key keyboard key to check
        static bool is_key_held(sf::Keyboard::Key key) {
            // check if keyboard is in held set
            held_lock.lock();
            bool out = held.count(key) > 0;
            held_lock.unlock();

            return out;
        }

        friend void SPlat::key_press_handler(std::string);

        friend void SPlat::key_release_handler(std::string);

    };

    /// @brief event of pressing a keyboard key
    class KeyPressEvent : public KeyEvent {

    public:

        /// @brief unique identifier for key press events
        const std::string KEY_PRESS_EVENT_TAG = "key_press";

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

            this->type = KEY_PRESS_EVENT_TAG;
            this->args = ss.str();
        }

    };

    /// @brief event of releasing a keyboard key
    class KeyReleaseEvent : public KeyEvent {

    public:

        /// @brief unique identifier for key release events
        const std::string KEY_RELEASE_EVENT_TAG = "key_release";

        /// @brief convert keyrelease to serializable event
        /// @param key keyboard key released
        KeyReleaseEvent(sf::Keyboard::Key key) {
            // Serialize key to JSON string
            KeyEventArgs args = {key};
            std::stringstream ss;
            {
                cereal::JSONOutputArchive oar(ss);
                oar(args);
            }

            this->type = KEY_RELEASE_EVENT_TAG;
            this->args = ss.str();
        }

    };

};

#endif
