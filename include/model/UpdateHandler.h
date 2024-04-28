#ifndef SPLAT_MODEL_UPDATEHANDLER_H
#define SPLAT_MODEL_UPDATEHANDLER_H

#include "model/MovingProperties.h"

namespace SPlat {

    namespace Model {

        class UpdateHandler {

            MovingProperties * properties = nullptr;

        public:

            UpdateHandler() = default;

            void set_properties(MovingProperties* properties) 
                { this->properties = properties; }

            virtual void update(time_t) = 0;

        };

    }

}

#endif