#ifndef SPlat_Events_ServerUpdateAssetHandler_h
#define SPlat_Events_ServerUpdateAssetHandler_h

#include "events/handlers/UpdateAssetHandler.h"

namespace SPlat {

    namespace Events {

        class ServerUpdateAssetHandler : public UpdateAssetHandler {

        public:

            void handle(std::string) override;

            static std::string get_type() { return UpdateAssetHandler::get_type(); }

        };

    }

}

#endif