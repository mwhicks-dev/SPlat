#ifndef SPLAT_PLATFORM
#define SPLAT_PLATFORM

#include "Asset.h"

namespace SPlat {

    class Platform : public Asset {

    public:

        Platform(sf::Vector2f&);

        int get_priority() override;

    };

}

#endif