#ifndef SPlat_PlatformCollisionHandler_h
#define SPlat_PlatformCollisionHandler_h

#include "model/AssetProperties.h"
#include "model/handler/CollisionHandler.h"

namespace SPlat {

    namespace Model {

        class PlatformCollisionHandler : public CollisionHandler {

            std::mutex m;

            AssetProperties& asset_properties;

            AssetProperties& get_asset_properties();

        public:

            PlatformCollisionHandler(AssetProperties& asset_properties) 
                    : asset_properties(asset_properties) {}
            
            void resolve_collision(AssetProperties&) override;

        };

    }

};

#endif