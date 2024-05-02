#ifndef SPlat_Events_ClientDeleteAssetHandler_h
#define SPlat_Events_ClientDeleteAssetHandler_h

#include "events/handlers/DeleteAssetHandler.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class ClientDeleteAssetHandler : public DeleteAssetHandler {

        public:

            void handle(std::string) override;

            static std::string get_type() { return DeleteAssetHandler::get_type(); }

        };

    }

}

#endif
