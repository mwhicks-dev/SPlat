#ifndef SPLAT_MODEL_COLLISIONHANDLER_H
#define SPLAT_MODEL_COLLISIONHANDLER_H

#include "model/AssetProperties.h"

namespace SPlat {

    namespace Model {

        class CollisionHandler {

            AssetProperties& properties;

        public:

            CollisionHandler(AssetProperties& properties)
            : properties(properties) {}

            virtual void resolve_collision(AssetProperties& other) = 0;

        };

    }

}

#endif