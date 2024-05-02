#ifndef ServerReadAssetHandler_h
#define ServerReadAssetHandler_h

#include "events/handlers/ReadAssetHandler.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class ServerReadAssetHandler : public ReadAssetHandler {

        public:

            void handle(std::string) override;

            static std::string get_type() { return ReadAssetHandler::get_type(); }

        };

    }

}

#endif
