#ifndef SPlat_ClientSpawnAssetHandler_h
#define SPlat_ClientSpawnAssetHandler_h

#include "events/handlers/SpawnAssetHandler.h"

namespace SPlat {

    namespace Events {

        class ClientSpawnAssetHandler : public SpawnAssetHandler {

        public:

            void handle(std::string) {}

        };

    }

}

#endif