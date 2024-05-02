#ifndef SPlat_Events_ServerCreateCharacterHandler_h
#define SPlat_Events_ServerCreateCharacterHandler_h

#include "events/handlers/EventHandlerInterface.h"

namespace SPlat {

    namespace Events {

        class ServerCreatCharacterHandler : public EventHandlerInterface {

        public:

            void handle(std::string);

        };

    }

}

#endif