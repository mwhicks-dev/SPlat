#ifndef SPlat_Events_ClientCreatePlatformHandler_h
#define SPlat_Events_ClientCreatePlatformHandler_h

#include "events/handlers/EventHandlerInterface.h"
#include "model/AssetProperties.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class ClientCreatePlatformHandler : public EventHandlerInterface {

        public:

            struct Args {

                SPlat::Model::AssetProperties properties;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(properties);
                }

            };

            void handle(std::string) override;

            static std::string get_type() { return "create_platform"; }

        };

    }

}

#endif