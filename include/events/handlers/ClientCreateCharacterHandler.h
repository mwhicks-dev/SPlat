#ifndef SPlat_Events_ClientCreateCharacterHandler_h
#define SPlat_Events_ClientCreateCharacterHandler_h

#include "events/handlers/CreateCharacterHandler.h"
#include "model/AssetProperties.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class ClientCreateCharacterHandler : public CreateCharacterHandler {

        public:

            void handle(std::string) override;

            static std::string get_type() { return CreateCharacterHandler::get_type(); }

        };

    }

}

#endif