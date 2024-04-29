#ifndef SPlat_AssetFactoryConfig_h
#define SPlat_AssetFactoryConfig_h

#include "AbstractAssetFactoryConfig.h"

namespace SPlat {

    class AssetFactoryConfig : public AbstractAssetFactoryConfig {

        std::mutex m;

        Model::AbstractAssetFactory& character_factory;

        Model::AbstractAssetFactory& platform_factory;

        Model::AbstractAssetFactory& moving_platform_factory;

    public:

        AssetFactoryConfig();

        Model::AbstractAssetFactory& get_character_factory() override;

        Model::AbstractAssetFactory& get_platform_factory() override;

        Model::AbstractAssetFactory& get_moving_platform_factory() override;

    };

}

#endif