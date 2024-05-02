#ifndef SPlat_Events_ServerUpdateAssetHandler_h
#define SPlat_Events_ServerUpdateAssetHandler_h

#include "events/handlers/EventHandlerInterface.h"

namespace SPlat {

    namespace Events {

        class ServerUpdateAssetHandler : public EventHandlerInterface {

        public:

            void handle(std::string);

        };

    }

}

#endif