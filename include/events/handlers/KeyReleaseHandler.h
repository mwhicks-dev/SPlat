#ifndef SPlat_KeyReleaseHandler_h
#define SPlat_KeyReleaseHandler_h

#include "events/handlers/EventHandlerInterface.h"

#include <SFML/Window/Keyboard.hpp>

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class KeyReleaseHandler : public EventHandlerInterface {

        public:

            struct Args {

                sf::Keyboard::Key key;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(key);
                }

            };

            void handle(std::string) override;

            static std::string get_event_type() { return "key_release"; }

        };

    }

};


#endif