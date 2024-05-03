#ifndef SPlat_CreateMovingPlatformHandler_h
#define SPlat_CreateMovingPlatformHandler_h

#include "events/handlers/EventHandlerInterface.h"
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

            virtual void handle(std::string) = 0;

            static std::string get_type() { return "create_moving_platform"; }

        };

    }

}

#endif