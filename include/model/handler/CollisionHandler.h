#ifndef SPLAT_MODEL_COLLISIONHANDLER_H
#define SPLAT_MODEL_COLLISIONHANDLER_H

#include "model/AssetProperties.h"

namespace SPlat {

    namespace Model {

        class CollisionHandler {

        public:

            virtual void resolve_collision(AssetProperties&) = 0;

        };

    }

}

#endif