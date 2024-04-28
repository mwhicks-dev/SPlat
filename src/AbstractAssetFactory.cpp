#include "model/AbstractAssetFactory.h"
#include "model/GameObjectModel.h"

using namespace SPlat::Model;

Asset& AbstractAssetFactory::read_asset(size_t id) {
    return GameObjectModel::get_instance().read_asset(id);
}

Asset& AbstractAssetFactory::delete_asset(size_t id) {
    return GameObjectModel::get_instance().delete_asset(id);
}