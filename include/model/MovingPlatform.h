#ifndef SPLAT_MOVING_PLATFORM
#define SPLAT_MOVING_PLATFORM

#include "model/Moving.h"
#include "model/MovingPlatformProperties.h"

namespace SPlat {

    namespace Model {

        class MovingPlatform : public Moving {

            MovingPlatformProperties properties;

        public:

            MovingPlatform(MovingPlatformProperties properties, 
                    CollisionHandler& collision_handler,
                    UpdateHandler& update_handler) : Moving(properties, 
                    collision_handler, update_handler), 
                    properties(properties) {};
            
            AssetProperties& get_asset_properties() override { return properties; }
            
            MovingProperties& get_moving_properties() override { return properties; }

        };

    }

}

#endif