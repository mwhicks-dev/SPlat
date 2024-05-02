#ifndef SPlat_Events_ClientCreateCharacterHandler_h
#define SPlat_Events_ClientCreateCharacterHandler_h

#include "events/handlers/EventHandlerInterface.h"
#include "model/AssetProperties.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class ClientCreateCharacterHandler : public EventHandlerInterface {

        public:

            struct Args {

                SPlat::Model::AssetProperties properties;

                bool set_controlled = false;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(properties, set_controlled);
                }

            };

            void handle(std::string) override;

            static std::string get_type() { return "create_character"; }

        };

    }

}

#endif