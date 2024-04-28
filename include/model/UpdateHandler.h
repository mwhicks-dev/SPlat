#ifndef SPLAT_MODEL_UPDATEHANDLER_H
#define SPLAT_MODEL_UPDATEHANDLER_H

#include "model/Moving.h"

namespace SPlat {

    namespace Model {

        class UpdateHandler {

            Moving::Properties& properties;

        public:

            UpdateHandler(Moving::Properties& properties)
            : properties(properties) {}

            virtual void update(time_t) = 0;

        };

    }

}

#endif