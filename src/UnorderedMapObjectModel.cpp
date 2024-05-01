#include "model/UnorderedMapObjectModel.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

bool UnorderedMapObjectModel::causes_collision(size_t id) {
    m.lock();
    bool local = asset_ids.count(id) > 0;
    m.unlock();

    return local;
}

void UnorderedMapObjectModel::add_id(size_t id) {
#ifdef DEBUG
    std::cout << "-> UnorderedMapObjectModel::add_id(" << id << ")" << std::endl;
#endif
    m.lock();
    asset_ids.insert(id);
    m.unlock();
#ifdef DEBUG
    std::cout << "<- UnorderedMapObjectModel::add_id" << std::endl;
#endif
}

void UnorderedMapObjectModel::remove_id(size_t id) {
#ifdef DEBUG
    std::cout << "-> UnorderedMapObjectModel::remove_id(" << id << ")" << std::endl;
#endif
    m.lock();
    asset_ids.erase(id);
    m.unlock();
#ifdef DEBUG
    std::cout << "<- UnorderedMapObjectModel::remove_id" << std::endl;
#endif
}

void UnorderedMapObjectModel::add_asset(size_t id, Asset* ptr) {
#ifdef DEBUG
    std::cout << "-> UnorderedMapObjectModel::add_asset(" << id << ", Asset*)" << std::endl;
#endif
    m.lock();
    game_assets[id] = ptr;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- UnorderedMapObjectModel::add_asset" << std::endl;
#endif
}

void UnorderedMapObjectModel::remove_asset(size_t id) {
#ifdef DEBUG
    std::cout << "-> UnorderedMapObjectModel::remove_asset(" << id << ")" << std::endl;
#endif
    m.lock();
    game_assets.erase(id);
    m.unlock();
#ifdef DEBUG
    std::cout << "<- UnorderedMapObjectModel::remove_asset" << std::endl;
#endif
}

Asset& UnorderedMapObjectModel::create_asset(Asset& asset) {
    // add to persistent state
    size_t id = asset.get_asset_properties().get_id();
    add_id(id);
    add_asset(id, &asset);

    return asset;
}

Asset& UnorderedMapObjectModel::read_asset(size_t id) {
    if (!causes_collision(id))
        throw std::invalid_argument("No asset with id " + id);
    
    m.lock();
    Asset& local = *game_assets[id];
    m.unlock();

    return local;
}

Asset& UnorderedMapObjectModel::delete_asset(size_t id) {
    Asset& local = read_asset(id);

    remove_id(id);
    remove_asset(id);

    return local;
}

std::unordered_set<size_t> UnorderedMapObjectModel::get_ids() {
    m.lock();
    auto local = asset_ids;
    m.unlock();

    return local;
}