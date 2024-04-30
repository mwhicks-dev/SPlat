#ifndef SPlat_Events_ListenerInterface_h
#define SPlat_Events_ListenerInterface_h

#include "events/Command.h"

namespace SPlat {

    namespace Events {

        class ListenerInterface {

        public:

            virtual void push_command(Command) = 0;

            virtual void run() = 0;

        };

    }

}

#endif