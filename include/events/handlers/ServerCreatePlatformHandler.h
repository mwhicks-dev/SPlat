#ifndef SPlat_Events_ServerCreatePlatformHandler_h
#define SPlat_Events_ServerCreatePlatformHandler_h

#include "events/handlers/CreatePlatformHandler.h"

namespace SPlat {

    namespace Events {

        class ServerCreatePlatformHandler : public CreatePlatformHandler {

        public:

            void handle(std::string);

            static std::string get_type() { return CreatePlatformHandler::get_type(); }

        };

    }

}

#endif