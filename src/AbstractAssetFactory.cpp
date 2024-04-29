#include "model/AbstractAssetFactory.h"
#include "model/GameObjectModel.h"

#include <cmath>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

Asset& AbstractAssetFactory::read_asset(size_t id) {
    return GameObjectModel::get_instance().read_asset(id);
}

Asset& AbstractAssetFactory::update_asset(size_t id, AssetProperties& properties) {
    Asset& asset = read_asset(id);

    AssetProperties& asset_properties = asset.get_asset_properties();

    asset_properties.set_size(properties.get_size());
    asset_properties.set_position(properties.get_position());
    asset_properties.set_fill_color(properties.get_fill_color());

    return asset;
}

Asset& AbstractAssetFactory::delete_asset(size_t id) {
    return GameObjectModel::get_instance().delete_asset(id);
}
