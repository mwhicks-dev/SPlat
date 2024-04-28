#ifndef SPLAT_MODEL_COLLISIONHANDLER_H
#define SPLAT_MODEL_COLLISIONHANDLER_H

#include "model/AssetProperties.h"

namespace SPlat {

    namespace Model {

        class CollisionHandler {

            AssetProperties * properties = nullptr;

        public:

            CollisionHandler() = default;

            void set_properties(AssetProperties* properties) {
                properties->m.lock();
                this->properties = properties;
                properties->m.unlock();
            }

            virtual void resolve_collision(AssetProperties& other) = 0;

        };

    }

}

#endif