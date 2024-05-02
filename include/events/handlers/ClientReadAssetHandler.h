#ifndef SPlat_Events_ClientReadAssetHandler_h
#define SPlat_Events_ClientReadAssetHandler_h

#include "events/handlers/ReadAssetHandler.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class ClientReadAssetHandler : public ReadAssetHandler {

        public:

            void handle(std::string) override;

            static std::string get_type() { return ReadAssetHandler::get_type(); }

        };

    }

}

#endif
