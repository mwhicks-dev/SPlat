#ifndef SPlat_KeyReleaseCommandHandler_h
#define SPlat_KeyReleaseCommandHandler_h

#include "CommandHandlerInterface.h"

#include <SFML/Window/Keyboard.hpp>

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class KeyReleaseCommandHandler : public CommandHandlerInterface {

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