#ifndef SPLAT_CHARACTER
#define SPLAT_CHARACTER

#include "Asset.h"

namespace SPlat {

    namespace Model {

        class Character : public Asset {

        public:

            static std::string TYPE;

            Character(sf::Vector2f&);

            int get_priority() override;

            std::string get_type() override;

        };

    }

}

#endif