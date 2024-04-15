#ifndef SPLAT_PLATFORM
#define SPLAT_PLATFORM

#include "Asset.h"

namespace SPlat {

    namespace Model {

        class Platform : public Asset {

        public:

            static std::string TYPE;

            Platform(sf::Vector2f&);

            int get_priority() override;

            std::string get_type() override;

        };

    }

}

#endif