#ifndef SPlat_MovingPlatformCollisionHandler_h
#define SPlat_MovingPlatformCollisionHandler_h

#include "model/AssetProperties.h"
#include "model/MovingProperties.h"
#include "model/MovingPlatformProperties.h"
#include "model/handler/CollisionHandler.h"

namespace SPlat {

    namespace Model {

        class MovingPlatformCollisionHandler : public CollisionHandler {

            std::mutex m;

            AssetProperties& asset;

            MovingProperties& moving;

            MovingPlatformProperties& moving_platform;

            AssetProperties& get_asset_properties();

            MovingProperties& get_moving_properties();

            MovingPlatformProperties& get_moving_platform_properties();

        public:

            MovingPlatformCollisionHandler(AssetProperties& asset, 
                    MovingProperties& moving, MovingPlatformProperties& 
                    moving_platform) : asset(asset), moving(moving), 
                    moving_platform(moving_platform) {}
            
            void resolve_collision(AssetProperties&) override;

        };

    }

};

#endif