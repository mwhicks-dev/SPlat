#ifndef SPLAT_MODEL_CHARACTERFACTORY_H
#define SPLAT_MODEL_CHARACTERFACTORY_H

#include "model/AbstractAssetFactory.h"

namespace SPlat {

    namespace Model {

        class CharacterFactory : public AbstractAssetFactory {

            Asset& create_asset(AssetProperties&) override;

        };

    }

}

#endif