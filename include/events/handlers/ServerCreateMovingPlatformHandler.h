#ifndef SPlat_Events_ServerCreateMovingPlatformHandler_h
#define SPlat_Events_ServerCreateMovingPlatformHandler_h

#include "events/handlers/CreateMovingPlatformHandler.h"

namespace SPlat {

    namespace Events {

        class ServerCreateMovingPlatormHandler : public CreateMovingPlatformHandler {

        public:

            void handle(std::string);

            static std::string get_type() { return CreateMovingPlatformHandler::get_type(); }

        };

    }

}

#endif