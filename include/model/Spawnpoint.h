#ifndef SPlat_Model_Spawnpoint_h
#define SPlat_Model_Spawnpoint_h

#include "model/Asset.h"

namespace SPlat {

    namespace Model {

        class Spawnpoint : public Asset {

        public:

            Spawnpoint(AssetProperties& properties) : Asset(properties) {
                properties.set_loaded(false);
                properties.set_drawable(false);
            }

            void resolve_collision(Asset& other) {}  // no collision behavior

            constexpr static int collision_priority() { return 9999; }  // lowest priority possible

        };

    }

}

#endif