#ifndef SPLAT_PLATFORM
#define SPLAT_PLATFORM

#include "model/Asset.h"

namespace SPlat {

    namespace Model {

        class Platform : public Asset {

            std::mutex m;

            CollisionHandler * collision_handler = nullptr;

            AssetProperties& properties;

        public:

            Platform(AssetProperties& properties) : Asset(properties), properties(properties) {};
            
            void resolve_collision(Asset& other);

        };

    }

}

#endif