#ifndef SPLAT_MOVING_PLATFORM
#define SPLAT_MOVING_PLATFORM

#include "Platform.h"

namespace SPlat {

    namespace Model {

        class MovingPlatform : public Platform {

        public:

            static std::string TYPE;

            MovingPlatform(sf::Vector2f&);

            int get_priority() override;

            std::string get_type() override;

        };

    }

}

#endif