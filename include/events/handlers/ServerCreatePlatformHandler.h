#ifndef SPlat_Events_ServerCreatePlatformHandler_h
#define SPlat_Events_ServerCreatePlatformHandler_h

#include "events/handlers/EventHandlerInterface.h"

namespace SPlat {

    namespace Events {

        class ServerCreatePlatormHandler : public EventHandlerInterface {

        public:

            void handle(std::string);

        };

    }

}

#endif