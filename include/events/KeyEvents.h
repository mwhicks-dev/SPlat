#ifndef SPLAT_KEYEVENTS_H
#define SPLAT_KEYEVENTS_H

#include "events/Event.h"

#include <SFML/Graphics.hpp>

#include <set>
#include <mutex>

namespace SPlat {

    namespace Events {

        /// @brief event that makes changes to what keys are stored as held
        class KeyEvent : public Event {

        protected:

            /// @brief mutex guarding access to keys_held
            static std::mutex keys_held_lock;

            /// @brief set of currently held keyboard keys
            static std::set<sf::Keyboard::Key> keys_held;

            /// @brief key modified by event
            sf::Keyboard::Key key;

        public:

            /// @brief arguments for key events
            struct Args {

                /// @brief key modified by event
                sf::Keyboard::Key key;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(key);
                }

            };

            /// @brief constructs new KeyEvent from key
            /// @param key key modified by event
            KeyEvent(sf::Keyboard::Key);

            static std::string get_type() 
                { throw std::domain_error("Not implemented"); }

            static bool is_key_pressed(sf::Keyboard::Key);

        };

        /// @brief event raised when a key is pressed
        class KeyPressEvent : public KeyEvent {

        public:

            /// @brief constructs a new KeyPressEvent from key
            /// @param key pressed keyboard key
            KeyPressEvent(sf::Keyboard::Key key) : KeyEvent(key) {}

            static std::string get_type() { return "key_press_event"; }

            void raise() override;

            static void handler(std::string);

        };

        /// @brief event raised when a key is released
        class KeyReleaseEvent : public KeyEvent {

        public:

            /// @brief constructs a new KeyReleaseEvent from key
            /// @param key released keyboard key
            KeyReleaseEvent(sf::Keyboard::Key key) : KeyEvent(key) {}

            static std::string get_type() { return "key_press_event"; }

            void raise() override;

            static void handler(std::string);

        };

    }

};

#endif
