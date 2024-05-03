#ifndef SPLAT_MOVING_PLATFORM
#define SPLAT_MOVING_PLATFORM

#include "model/Moving.h"
#include "model/MovingPlatformProperties.h"

namespace SPlat {

    namespace Model {

        class MovingPlatform : public Moving {

            std::mutex m;

            MovingPlatformProperties& properties;

        public:

            MovingPlatform(AssetProperties& asset_properties, MovingProperties&
                    moving_properties, MovingPlatformProperties& 
                    moving_platform_properties) : Moving(asset_properties,
                    moving_properties), properties(moving_platform_properties) 
                    {};

            MovingPlatformProperties& get_moving_platform_properties() {
                const std::lock_guard<std::mutex> lock(m);
                return properties;
            }

            void resolve_collision(Asset& other) override;

            void update() override;

            static constexpr int collision_priority() { return -1; }

        };

    }

}

#endif