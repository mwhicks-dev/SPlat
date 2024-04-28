#ifndef SPLAT_MODEL_MOVINGPLATFORMFACTORY_H
#define SPLAT_MODEL_MOVINGPLATFORMFACTORY_H

#include "model/AbstractMovingFactory.h"

namespace SPlat {

    namespace Model {

        class MovingPlatformFactory : public AbstractMovingFactory {

        public:

            Asset& create_asset(AssetProperties&) override;

            Asset& update_asset(size_t, AssetProperties&) override;

        };

        class DefaultUpdateHandler : public UpdateHandler {

        public:

            void update(time_t) override;

        };

    }

}

#endif