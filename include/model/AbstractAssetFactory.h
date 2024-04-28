#ifndef SPLAT_MODEL_ABSTRACTASSETFACTORY_H
#define SPLAT_MODEL_ABSTRACTASSETFACTORY_H

#include "model/Asset.h"

namespace SPlat {

    namespace Model {

        class AbstractAssetFactory {

            virtual Asset& create_asset(AssetProperties&) = 0;

            Asset& read_asset(size_t);

            virtual Asset& update_asset(size_t, AssetProperties&) = 0;

            Asset& delete_asset(size_t);

        };

    }

}

#endif