#include "AssetFactoryConfig.h"
#include "model/CharacterFactory.h"
#include "model/PlatformFactory.h"
#include "model/MovingPlatformFactory.h"

using namespace SPlat;

AssetFactoryConfig::AssetFactoryConfig() :
character_factory(*new Model::CharacterFactory()),
platform_factory(*new Model::PlatformFactory()),
moving_platform_factory(*new Model::MovingPlatformFactory()) {}

Model::AbstractAssetFactory& AssetFactoryConfig::get_character_factory() {
    const std::lock_guard<std::mutex> lock(m);
    Model::AbstractAssetFactory& local = character_factory;
    
    return local;
}

Model::AbstractAssetFactory& AssetFactoryConfig::get_platform_factory() {
    const std::lock_guard<std::mutex> lock(m);
    Model::AbstractAssetFactory& local = platform_factory;
    
    return local;
}

Model::AbstractAssetFactory& AssetFactoryConfig::get_moving_platform_factory() {
    const std::lock_guard<std::mutex> lock(m);
    Model::AbstractAssetFactory& local = moving_platform_factory;
    
    return local;
}
