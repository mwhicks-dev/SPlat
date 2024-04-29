#ifndef SPlat_PlatformCollisionHandler_h
#define SPlat_PlatformCollisionHandler_h

#include "model/AssetProperties.h"
#include "model/CollisionHandler.h"

namespace SPlat {

    namespace Model {

        class PlatformCollisionHandler : public CollisionHandler {

            AssetProperties& asset;

        public:

            PlatformCollisionHandler(AssetProperties& asset) 
                    : asset(asset) {}
            
            void resolve_collision(AssetProperties& other) override;

        };

    }

};

#endif