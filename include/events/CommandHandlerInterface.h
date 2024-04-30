#ifndef SPlat_CommandHandlerInterface_h
#define SPlat_CommandHandlerInterface_h

#include <string>

namespace SPlat {

    namespace Events {

        class CommandHandlerInterface {

        public:

            virtual void handle(std::string) = 0;

        };

    }

}

#endif