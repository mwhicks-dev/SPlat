#ifndef SPlat_MovingPlatformUpdateHandler_h
#define SPlat_MovingPlatformUpdateHandler_h

#include "model/AssetProperties.h"
#include "model/MovingProperties.h"
#include "model/MovingPlatformProperties.h"
#include "model/UpdateHandler.h"

namespace SPlat {

    namespace Model {

        class MovingPlatformUpdateHandler : public UpdateHandler {

            AssetProperties& asset;

            MovingProperties& moving;

            MovingPlatformProperties& moving_platform;

        public:

            MovingPlatformUpdateHandler(AssetProperties& asset, 
                    MovingProperties& moving, MovingPlatformProperties& 
                    moving_platform) : asset(asset), moving(moving), 
                    moving_platform(moving_platform) {}
            
            void update() override;

        };

    }

};

#endif