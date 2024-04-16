#include "model/GameObjectModel.h"

using namespace SPlat::Model;

Asset& GameObjectModel::create_asset(Asset& asset) {
    lock.lock();

    // Get Asset ID
    size_t id = assets.size();
    while (assets.count(id) > 0) id++;
    ids.insert(id);

    // Assign ID and store
    asset.id = id;
    assets[id] = &asset;

    lock.unlock();

    return asset;
}

Asset& GameObjectModel::read_asset(size_t id) {
    lock.lock();

    if (assets.count(id) == 0) {
        lock.unlock();
        throw std::invalid_argument("No asset with id " + id);
    }
    
    Asset& asset = *assets[id];

    lock.unlock();

    return asset;
}

Asset& GameObjectModel::update_asset(size_t id, Asset& update) {
    lock.lock();

    if (assets.count(id) == 0) {
        lock.unlock();
        throw std::invalid_argument("No asset with id " + id);
    }

    Asset& asset = *assets[id];

    asset.setPosition(update.getPosition());
    asset.setTexture(update.getTexture());
    asset.setTextureRect(update.getTextureRect());

    // NOTE: update velocity is ADDED to original
    asset.velocity += update.velocity;

    lock.unlock();

    return asset;
}

Asset& GameObjectModel::delete_asset(size_t id) {
    lock.lock();

    if (assets.count(id) == 0) {
        lock.unlock();
        throw std::invalid_argument("No asset with id " + id);
    }

    Asset& asset = *assets[id];

    assets.erase(id);
    ids.erase(id);

    lock.unlock();

    return asset;
}