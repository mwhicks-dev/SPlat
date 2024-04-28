#ifndef SPLAT_MODEL_ASSET_H
#define SPLAT_MODEL_ASSET_H

#include "model/CollisionHandler.h"

namespace SPlat {

    namespace Model {

        class Asset {

            CollisionHandler * collision_handler = nullptr;

        public:

            Asset(AssetProperties properties, CollisionHandler& handler) {
                handler.set_properties(&get_asset_properties());
                set_collision_handler(handler);
            }

            void set_collision_handler(CollisionHandler& handler) {
                collision_handler = &handler;
            }

            void resolve_collision(Asset& other) {
                collision_handler->resolve_collision(other.get_asset_properties());
            }

            virtual AssetProperties& get_asset_properties() = 0;

        };

    }

}

#endif