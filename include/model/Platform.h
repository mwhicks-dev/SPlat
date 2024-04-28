#ifndef SPLAT_PLATFORM
#define SPLAT_PLATFORM

#include "model/Asset.h"

namespace SPlat {

    namespace Model {

        class Platform : public Asset {

            AssetProperties properties;

        public:

            Platform(AssetProperties properties, 
                    CollisionHandler& collision_handler) : Asset(properties, 
                    collision_handler), properties(properties) {};
            
            AssetProperties& get_asset_properties() override { return properties; }

        };

    }

}

#endif