#ifndef SPlat_Events_CreatePlatformHandler_h
#define SPlat_Events_CreatePlatformHandler_h

#include "events/CommandHandlerInterface.h"
#include "model/AssetProperties.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class CreatePlatformHandler : public CommandHandlerInterface {

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
