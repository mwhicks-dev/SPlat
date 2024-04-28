#ifndef SPLAT_MODEL_COLLISIONHANDLER_H
#define SPLAT_MODEL_COLLISIONHANDLER_H

#include "model/Asset.h"

namespace SPlat {

    namespace Model {

        class CollisionHandler {

            Asset::Properties& properties;

        public:

            CollisionHandler(Asset::Properties& properties)
            : properties(properties) {}

            virtual void resolve_collision(Asset& other) = 0;

        };

    }

}

#endif