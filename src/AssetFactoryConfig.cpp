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
    m.lock();
    Model::AbstractAssetFactory& local = character_factory;
    m.unlock();

    return local;
}

Model::AbstractAssetFactory& AssetFactoryConfig::get_platform_factory() {
    m.lock();
    Model::AbstractAssetFactory& local = platform_factory;
    m.unlock();

    return local;
}

Model::AbstractAssetFactory& AssetFactoryConfig::get_moving_platform_factory() {
    m.lock();
    Model::AbstractAssetFactory& local = moving_platform_factory;
    m.unlock();

    return local;
}
