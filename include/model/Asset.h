#ifndef SPLAT_MODEL_ASSET_H
#define SPLAT_MODEL_ASSET_H

#include "model/CollisionHandler.h"

namespace SPlat {

    namespace Model {

        class Asset {

            AssetProperties properties;

            CollisionHandler * collision_handler = nullptr;

        public:

            Asset(AssetProperties properties, CollisionHandler& handler)
            : properties(properties) {
                set_collision_handler(handler);
            }

            void set_collision_handler(CollisionHandler& handler) {
                collision_handler = &handler;
                collision_handler->set_properties(&properties);
            }

            void resolve_collision(Asset& other) {
                collision_handler->resolve_collision(other.properties);
            }

        };

    }

}

#endif