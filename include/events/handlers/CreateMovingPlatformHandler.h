#ifndef SPlat_Events_CreateMovingPlatformHandler_h
#define SPlat_Events_CreateMovingPlatformHandler_h

#include "events/EventHandlerInterface.h"
#include "model/AssetProperties.h"
#include "model/MovingPlatformProperties.h"

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

namespace SPlat {

    namespace Events {

        class CreateMovingPlatformHandler : public EventHandlerInterface {

        public:

            struct Args {

                SPlat::Model::AssetProperties properties;

                std::vector<SPlat::Model::State> states;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(properties, states);
                }

            };

            void handle(std::string) override;

            static std::string get_type() { return "create_moving_platform"; }

        };

    }

}

#endif
