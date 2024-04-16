#ifndef SPLAT_KEYEVENTS_H
#define SPLAT_KEYEVENTS_H

#include <set>
#include <mutex>

#include <SFML/Window/Keyboard.hpp>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

#include "Event.h"
#include "events/Listener.h"

namespace SPlat {

    namespace Events {

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

        };

        /// @brief event of pressing a keyboard key
        class KeyPressEvent : public KeyEvent {

        public:

            /// @brief unique identifier for key press events
            static std::string TYPE;

            static void handler(std::string);

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

                this->type = TYPE;
                this->args = ss.str();
                this->foreground = true;
            }

        };

        /// @brief event of releasing a keyboard key
        class KeyReleaseEvent : public KeyEvent {

        public:

            /// @brief unique identifier for key release events
            static std::string TYPE;

            static void handler(std::string);

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

                this->type = TYPE;
                this->args = ss.str();

                // pass to foreground listener
                ForegroundListener &lst = ForegroundListener::get_instance();
                lst.push_event(*this);
            }

        };

    }

};

#endif
