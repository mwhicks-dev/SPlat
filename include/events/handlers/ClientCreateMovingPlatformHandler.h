#ifndef SPlat_Events_ClientCreateMovingPlatformHandler_h
#define SPlat_Events_ClientCreateMovingPlatformHandler_h

#include "events/handlers/CreateMovingPlatformHandler.h"
#include "model/AssetProperties.h"
#include "model/MovingPlatformProperties.h"

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

namespace SPlat {

    namespace Events {

        class ClientCreateMovingPlatformHandler : public CreateMovingPlatformHandler {

        public:

            void handle(std::string) override;

            static std::string get_type() { return CreateMovingPlatformHandler::get_type(); }

        };

    }

}

#endif
