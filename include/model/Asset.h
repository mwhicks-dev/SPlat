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
                m.lock();
                CollisionHandler * local = collision_handler;
                m.unlock();

                return local;
            }

        public:

            Asset(AssetProperties& properties) : properties(properties) {}

            void set_collision_handler(CollisionHandler * collision_handler) {
                m.lock();
                this->collision_handler = collision_handler;
                m.unlock();
            }

            AssetProperties& get_asset_properties() {
                m.lock();
                AssetProperties& local = properties;
                m.unlock();

                return local;
            }

            virtual void resolve_collision(Asset& other) = 0;

        };

    }

}

#endif