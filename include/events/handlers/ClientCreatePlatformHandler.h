#ifndef SPlat_Events_ClientCreatePlatformHandler_h
#define SPlat_Events_ClientCreatePlatformHandler_h

#include "events/handlers/CreatePlatformHandler.h"
#include "model/AssetProperties.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class ClientCreatePlatformHandler : public CreatePlatformHandler {

        public:

            void handle(std::string) override;

            static std::string get_type() { return CreatePlatformHandler::get_type(); }

        };

    }

}

#endif
