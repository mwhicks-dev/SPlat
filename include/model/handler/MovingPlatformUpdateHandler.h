#ifndef SPlat_MovingPlatformUpdateHandler_h
#define SPlat_MovingPlatformUpdateHandler_h

#include "model/AssetProperties.h"
#include "model/MovingProperties.h"
#include "model/MovingPlatformProperties.h"
#include "model/handler/UpdateHandler.h"

namespace SPlat {

    namespace Model {

        class MovingPlatformUpdateHandler : public UpdateHandler {

            std::mutex m;

            AssetProperties& asset;

            MovingProperties& moving;

            MovingPlatformProperties& moving_platform;

            AssetProperties& get_asset_properties();

            MovingProperties& get_moving_properties();

            MovingPlatformProperties& get_moving_platform_properties();

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