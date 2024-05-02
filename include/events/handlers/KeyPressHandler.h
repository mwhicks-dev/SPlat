#ifndef SPlat_KeyPressHandler_h
#define SPlat_KeyPressHandler_h

#include "events/handlers/EventHandlerInterface.h"

#include <SFML/Window/Keyboard.hpp>

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class KeyPressHandler : public EventHandlerInterface {

        public:

            struct Args {

                sf::Keyboard::Key key;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(key);
                }

            };

            void handle(std::string) override;

            static std::string get_event_type() { return "key_press"; }

        };

    }

};


#endif