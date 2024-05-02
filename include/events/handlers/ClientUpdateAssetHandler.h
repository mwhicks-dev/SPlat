#ifndef SPlat_Events_ClientUpdateAssetHandler_h
#define SPlat_Events_ClientUpdateAssetHandler_h

#include "events/handlers/UpdateAssetHandler.h"
#include "model/AssetProperties.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class ClientUpdateAssetHandler : public UpdateAssetHandler {

        public:

            void handle(std::string) override;

            static std::string get_type() { return UpdateAssetHandler::get_type(); }

        };

    }

}

#endif
