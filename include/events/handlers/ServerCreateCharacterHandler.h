#ifndef SPlat_Events_ServerCreateCharacterHandler_h
#define SPlat_Events_ServerCreateCharacterHandler_h

#include "events/handlers/CreateCharacterHandler.h"

namespace SPlat {

    namespace Events {

        class ServerCreatCharacterHandler : public CreateCharacterHandler {

        public:

            void handle(std::string);

            static std::string get_type() { return CreateCharacterHandler::get_type(); }

        };

    }

}

#endif