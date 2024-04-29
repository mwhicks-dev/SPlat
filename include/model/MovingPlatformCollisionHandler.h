#ifndef SPlat_MovingPlatformCollisionHandler_h
#define SPlat_MovingPlatformCollisionHandler_h

#include "model/AssetProperties.h"
#include "model/MovingProperties.h"
#include "model/MovingPlatformProperties.h"
#include "model/CollisionHandler.h"

namespace SPlat {

    namespace Model {

        class MovingPlatformCollisionHandler : public CollisionHandler {

            AssetProperties& asset;

            MovingProperties& moving;

            MovingPlatformProperties& moving_platform;

        public:

            MovingPlatformCollisionHandler(AssetProperties& asset, 
                    MovingProperties& moving, MovingPlatformProperties& 
                    moving_platform) : asset(asset), moving(moving), 
                    moving_platform(moving_platform) {}
            
            void resolve_collision(AssetProperties& other) override;

        };

    }

};

#endif