#ifndef SPlat_EventHandlerInterface_h
#define SPlat_EventHandlerInterface_h

#include <string>

namespace SPlat {

    namespace Events {

        class EventHandlerInterface {

        public:

            virtual void handle(std::string) = 0;

        };

    }

}

#endif