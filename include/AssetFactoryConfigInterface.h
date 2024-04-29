#ifndef SPlat_AssetFactoryConfigInterface_h
#define SPlat_AssetFactoryConfigInterface_h

#include "model/AbstractAssetFactory.h"

namespace SPlat {

    class AssetFactoryConfigInterface {

    public:

        virtual Model::AbstractAssetFactory& get_character_factory() = 0;

        virtual Model::AbstractAssetFactory& get_platform_factory() = 0;

        virtual Model::AbstractAssetFactory& get_moving_platform_factory() = 0;

    };

}

#endif