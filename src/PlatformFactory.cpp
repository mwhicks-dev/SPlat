#include "model/GameObjectModel.h"
#include "model/PlatformFactory.h"
#include "model/Platform.h"

using namespace SPlat::Model;

Asset& PlatformFactory::create_asset(AssetProperties& properties) {
    Platform * platform = new Platform(
        properties,
        *new AbstractAssetFactory::DefaultCollisionHandler()
    );
    return GameObjectModel::get_instance().create_asset(*platform);
}

Asset& PlatformFactory::update_asset(size_t id, AssetProperties& properties) {
    Platform& plt = (Platform&) GameObjectModel::get_instance().read_asset(id);

    AssetProperties& curr = plt.get_asset_properties();
    curr.set_fill_color(properties.get_fill_color());
    curr.set_position(properties.get_position());
    curr.set_size(properties.get_size());

    return plt;
}
