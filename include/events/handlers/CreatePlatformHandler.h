#ifndef SPlat_CreatePlatformHandler_h
#define SPlat_CreatePlatformHandler_h

#include "events/handlers/EventHandlerInterface.h"
#include "model/AssetProperties.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class CreatePlatformHandler : public EventHandlerInterface {

        public:

            struct Args {

                SPlat::Model::AssetProperties properties;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(properties);
                }

            };

            virtual void handle(std::string) = 0;

            static std::string get_type() { return "create_platform"; }

        };

    }

}

#endif