#ifndef SPLAT_MODEL_UPDATEHANDLER_H
#define SPLAT_MODEL_UPDATEHANDLER_H

#include "model/Properties.h"

namespace SPlat {

    namespace Model {

        class UpdateHandler {

            MovingProperties& properties;

        public:

            UpdateHandler(MovingProperties& properties)
            : properties(properties) {}

            virtual void update(time_t) = 0;

        };

    }

}

#endif