#ifndef SPLAT_MODEL_ASSET_H
#define SPLAT_MODEL_ASSET_H

#include "model/AssetProperties.h"
#include "model/handler/CollisionHandler.h"

namespace SPlat {

    namespace Model {

        class Asset {

            std::mutex m;

            AssetProperties& properties;

            CollisionHandler * collision_handler = nullptr;

        protected:

            CollisionHandler * get_collision_handler() {
                const std::lock_guard<std::mutex> lock(m);
                return collision_handler;
            }

        public:

            Asset(AssetProperties& properties) : properties(properties) {}

            void set_collision_handler(CollisionHandler * collision_handler) {
                const std::lock_guard<std::mutex> lock(m);
                this->collision_handler = collision_handler;
            }

            AssetProperties& get_asset_properties() {
                const std::lock_guard<std::mutex> lock(m);
                return properties;
            }

            virtual void resolve_collision(Asset& other) = 0;

        };

    }

}

#endif