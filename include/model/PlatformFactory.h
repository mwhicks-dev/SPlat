#ifndef SPLAT_MODEL_PLATFORMFACTORY_H
#define SPLAT_MODEL_PLATFORMFACTORY_H

#include "model/AbstractAssetFactory.h"

namespace SPlat {

    namespace Model {

        class PlatformFactory : public AbstractAssetFactory {

        public:

            Asset& create_asset(AssetProperties&) override;

        };

    }

}

#endif