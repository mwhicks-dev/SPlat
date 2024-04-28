#ifndef SPLAT_MODEL_ABSTRACTMOVINGFACTORY_H
#define SPLAT_MODEL_ABSTRACTMOVINGFACTORY_H

#include "model/AbstractAssetFactory.h"
#include "model/Moving.h"

namespace SPlat {

    namespace Model {

        class AbstractMovingFactory : public AbstractAssetFactory {

        public:

            class DefaultUpdateHandler : public UpdateHandler {

            public:

                void update(time_t) override;

            };

        };

    }

}

#endif