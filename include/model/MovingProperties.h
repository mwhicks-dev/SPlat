#ifndef SPLAT_MODEL_MOVINGPROPERTIES_H
#define SPLAT_MODEL_MOVINGPROPERTIES_H

#include "model/AssetProperties.h"

namespace SPlat {

    namespace Model {

        class MovingProperties : public AssetProperties {

        public:

            sf::Vector2f velocity;

            time_t last_updated;

        };

    }

}

#endif