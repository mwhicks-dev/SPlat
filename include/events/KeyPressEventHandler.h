#ifndef SPlat_KeyPressEventHandler_h
#define SPlat_KeyPressEventHandler_h

#include "EventHandlerInterface.h"

#include <SFML/Window/Keyboard.hpp>

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class KeyPressEventHandler : public EventHandlerInterface {

        public:

            struct Args {

                sf::Keyboard::Key key;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(key);
                }

            };

            void handle_event(std::string) override;

            static std::string get_event_type() { return "key_press_event"; }

        };

    }

};


#endif