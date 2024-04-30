#include "model/handler/UnorderedStandingConfig.h"
#include "model/GameObjectModel.h"

using namespace SPlat::Model;

std::unordered_map<size_t, std::unordered_set<size_t>> UnorderedStandingConfig
        ::get_coupled() {
    m.lock();
    std::unordered_map<size_t, std::unordered_set<size_t>> local = coupled;
    m.unlock();

    return coupled;
}

void UnorderedStandingConfig::set_coupled(std::unordered_map<size_t, 
        std::unordered_set<size_t>> coupled) {
    m.lock();
    this->coupled = coupled;
    m.unlock();
}

void UnorderedStandingConfig::push_child(size_t parent, size_t child) {
    auto local = get_coupled();
    
    if (local.count(parent) == 0) local[parent] = {};
    local[parent].insert(child);

    set_coupled(local);
}

void UnorderedStandingConfig::remove_child(size_t parent, size_t child) {
    auto local = get_coupled();

    if (local.count(parent) == 0) local[parent] = {};
    local[parent].erase(child);

    set_coupled(local);
}

void UnorderedStandingConfig::push_update_to_children(size_t parent, 
        sf::Vector2f update) {
    auto local = get_coupled();

    if (local.count(parent) == 0) return;
    auto children = local[parent];

    for (size_t child : children) {
        try {
            // get asset by ID
            Asset& child_asset = GameObjectModel::get_instance()
                .read_asset(child);

            // add update to child position
            AssetProperties& child_asset_properties = child_asset
                .get_asset_properties();
            child_asset_properties.set_position(child_asset_properties
                .get_position() + update);
        } catch (std::exception&) {}
    }
}
