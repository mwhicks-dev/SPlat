#ifndef SPlat_ServerSpawnAssetHandler_h
#define SPlat_ServerSpawnAssetHandler_h

#include "events/handlers/SpawnAssetHandler.h"

namespace SPlat {

    namespace Events {

        class ServerSpawnAssetHandler : public SpawnAssetHandler {

        public:

            void handle(std::string);

        };

    }

}

#endif