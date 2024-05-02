#ifndef SPlat_CreateCharacterHandler_h
#define SPlat_CreateCharacterHandler_h

#include "events/handlers/EventHandlerInterface.h"
#include "model/AssetProperties.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class CreateCharacterHandler : public EventHandlerInterface {

        public:

            struct Args {

                SPlat::Model::AssetProperties properties;

                bool set_controlled = false;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(properties, set_controlled);
                }

            };

            virtual void handle(std::string) = 0;

            static std::string get_type() { return "create_character"; }

        };

    }

}

#endif