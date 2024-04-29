#include "model/GameObjectModel.h"
#include "model/PlatformFactory.h"
#include "model/Platform.h"

using namespace SPlat::Model;

Asset& PlatformFactory::create_asset(AssetProperties& properties) {
    Platform * platform = new Platform(*new AssetProperties(properties));
    return GameObjectModel::get_instance().create_asset(*platform);
}
