#ifndef SPLAT_MODEL_MOVINGPLATFORMFACTORY_H
#define SPLAT_MODEL_MOVINGPLATFORMFACTORY_H

#include "model/AbstractAssetFactory.h"

namespace SPlat {

    namespace Model {

        class MovingPlatformFactory : public AbstractAssetFactory {

        public:

            Asset& create_asset(AssetProperties&) override;

        };

    }

}

#endif