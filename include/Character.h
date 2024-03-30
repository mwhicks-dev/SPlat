#ifndef SPLAT_CHARACTER
#define SPLAT_CHARACTER

#include "Asset.h"

namespace SPlat {

    class Character : public Asset {

    public:

        Character(sf::Vector2f&);

        int get_priority() override;

    };

}

#endif