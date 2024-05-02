#ifndef SPlat_Events_ServerDeleteAssetHandler_h
#define SPlat_Events_ServerDeleteAssetHandler_h

#include "events/handlers/DeleteAssetHandler.h"

namespace SPlat {

    namespace Events {

        class ServerDeleteAssetHandler : public DeleteAssetHandler {

        public:

            void handle(std::string);

            static std::string get_type() { return DeleteAssetHandler::get_type(); }

        };

    }

}

#endif