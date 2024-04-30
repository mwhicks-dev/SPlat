#include "model/PlatformFactory.h"
#include "model/Platform.h"
#include "Entrypoint.h"

using namespace SPlat::Model;

Asset& PlatformFactory::create_asset(AssetProperties& properties) {
    properties.set_collision_priority(-2);
    Platform * platform = new Platform(*new AssetProperties(properties));
    return Entrypoint::get_instance().get_object_model().create_asset(*platform);
}
