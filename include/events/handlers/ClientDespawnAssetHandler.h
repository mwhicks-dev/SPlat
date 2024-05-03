#ifndef SPlat_ClientDespawnAssetHandler_h
#define SPlat_ClientDespawnAssetHandler_h

#include "events/handlers/DespawnAssetHandler.h"

namespace SPlat {

    namespace Events {

        class ClientDespawnAssetHandler : public DespawnAssetHandler {

        public:

            void handle(std::string) {}

        };

    }

}

#endif