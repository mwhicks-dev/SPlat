#ifndef SPlat_Events_ServerDeleteAssetHandler_h
#define SPlat_Events_ServerDeleteAssetHandler_h

#include "events/handlers/EventHandlerInterface.h"

namespace SPlat {

    namespace Events {

        class ServerDeleteAssetHandler : public EventHandlerInterface {

        public:

            void handle(std::string);

        };

    }

}

#endif