#ifndef SPLAT_MODEL_ASSET_H
#define SPLAT_MODEL_ASSET_H

#include "AssetProperties.h"

namespace SPlat {

    namespace Model {

        class Asset {

            std::mutex m;

            AssetProperties& properties;

        public:

            Asset(AssetProperties& properties) : properties(properties) {}

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