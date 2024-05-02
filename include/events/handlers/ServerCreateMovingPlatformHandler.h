#ifndef SPlat_Events_ServerCreateMovingPlatformHandler_h
#define SPlat_Events_ServerCreateMovingPlatformHandler_h

#include "events/handlers/EventHandlerInterface.h"

namespace SPlat {

    namespace Events {

        class ServerCreateMovingPlatormHandler : public EventHandlerInterface {

        public:

            void handle(std::string);

        };

    }

}

#endif