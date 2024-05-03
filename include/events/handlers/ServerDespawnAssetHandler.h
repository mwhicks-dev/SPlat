#ifndef SPlat_ServerDespawnAssetHandler_h
#define SPlat_ServerDespawnAssetHandler_h

#include "events/handlers/DespawnAssetHandler.h"

namespace SPlat {

    namespace Events {

        class ServerDespawnAssetHandler : public DespawnAssetHandler {

        public:

            void handle(std::string);

        };

    }

}

#endif