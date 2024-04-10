#ifndef SPLAT_MOVING_PLATFORM
#define SPLAT_MOVING_PLATFORM

#include "Platform.h"

namespace SPlat {

    namespace Model {

        class MovingPlatform : public Platform {

        public:

            MovingPlatform(sf::Vector2f&);

            int get_priority() override;

        };

    }

}

#endif