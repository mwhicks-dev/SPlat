#include "model/GameObjectModel.h"
#include "model/Moving.h"

#include <cmath>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

Asset& GameObjectModel::create_asset(Asset& asset) {
    lock.lock();

    // Get Asset ID
    size_t id = assets.size();
    while (assets.count(id) > 0) id++;
    ids.insert(id);

    // Assign ID and store
    asset.get_asset_properties().set_id(id);
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

bool collision_checker(Asset& lhs, Asset& rhs) {
    return lhs.get_asset_properties().get_rectangle_shape().getGlobalBounds()
        .intersects(rhs.get_asset_properties().get_rectangle_shape()
        .getGlobalBounds());
}

void GameObjectModel::check_collision(size_t id) {
#ifdef DEBUG
    std::cout << "-> GameObjectModel::check_collision(" << id << ")" << std::endl;
#endif
    Asset * lhs = assets[id];

    if (lhs != nullptr) {
        for (size_t other_id : ids) {
            if (id == other_id) continue;

            Asset * rhs = assets[other_id];
            if (rhs == nullptr) continue;

            bool collision = collision_checker(*lhs, *rhs);
            
            if (!collision) continue;

            try {
                Moving& lhs_ref = (Moving&) *lhs;
                lhs_ref.resolve_collision(*rhs);
            } catch (std::exception&) {}
            try {
                Moving& rhs_ref = (Moving&) *rhs;
                rhs_ref.resolve_collision(*lhs);
            } catch (std::exception&) {}
        }
    }
#ifdef DEBUG
    std::cout << "<- GameObjectModel::check_collision" << std::endl;
#endif
}

std::unordered_set<size_t> GameObjectModel::get_ids() {
    lock.lock();
    auto local = ids;
    lock.unlock();

    return ids;
}

GameObjectModel& GameObjectModel::get_instance() {
    static GameObjectModel instance;
    return instance;
}