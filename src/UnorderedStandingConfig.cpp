#include "model/handler/UnorderedStandingConfig.h"
#include "events/handlers/UpdateAssetHandler.h"
#include "events/Command.h"
#include "Entrypoint.h"
#include "Event.h"

#include <cereal/archives/json.hpp>

#include <string>

#ifdef DEBUG
#include <iostream>
#endif

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
#ifdef DEBUG
    std::cout << "-> UnorderedStandingConfig::set_coupled(std::unordered_map<size_t, std::unordered_set<size_t>>)" << std::endl;
#endif
    m.lock();
    this->coupled = coupled;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- UnorderedStandingConfig::set_coupled" << std::endl;
#endif
}

void UnorderedStandingConfig::push_child(size_t parent, size_t child) {
#ifdef DEBUG
    std::cout << "-> UnorderedStandingConfig::push_child(" << parent << ", " << child << ")" << std::endl;
#endif
    auto local = get_coupled();
    
    if (local.count(parent) == 0) local[parent] = {};
    local[parent].insert(child);

    set_coupled(local);
#ifdef DEBUG
    std::cout << "<- UnorderedStandingConfig::push_child" << std::endl;
#endif
}

void UnorderedStandingConfig::remove_child(size_t parent, size_t child) {
#ifdef DEBUG
    std::cout << "-> UnorderedStandingConfig::remove_child(" << parent << ", " << child << ")" << std::endl;
#endif
    auto local = get_coupled();

    if (local.count(parent) == 0) local[parent] = {};
    local[parent].erase(child);

    set_coupled(local);
#ifdef DEBUG
    std::cout << "<- UnorderedStandingConfig::remove_child" << std::endl;
#endif
}

void UnorderedStandingConfig::push_update_to_children(size_t parent, 
        sf::Vector2f update) {
#ifdef DEBUG
    std::cout << "-> UnorderedStandingConfig::push_update_to_children(" << parent << ", sf::Vector2f)" << std::endl;
#endif
    Entrypoint& entrypoint = Entrypoint::get_instance();
    auto local = get_coupled();

    if (local.count(parent) == 0) return;
    auto children = local[parent];

    for (size_t child : children) {
        try {
            // get asset by ID
            Asset& child_asset = Entrypoint::get_instance().get_object_model().read_asset(child);

            // add update to child position
            AssetProperties& child_asset_properties = child_asset
                .get_asset_properties();
            child_asset_properties.set_position(child_asset_properties
                .get_position() + update);

            // raise event for persistence
            // raise update event for persistence
            Events::UpdateAssetHandler::Args args = {
                .id=child,
                .properties=child_asset_properties
            };
            std::stringstream ss;
            {
                cereal::JSONOutputArchive oar(ss);
                oar(args);
            }
            Events::Command cmd = {
                .priority=0,
                .type=Events::UpdateAssetHandler::get_type(),
                .args=ss.str(),
            };
            Event event = {
                .command=cmd,
                .sender=entrypoint.get_config().get_environment().get_entrypoint_id(),
            };
            ss.clear(); ss.str("");
            {
                cereal::JSONOutputArchive oar(ss);
                oar(event);
            }
            Request request = {
                .content_type=Request::ContentType::Event,
                .body=ss.str()
            };
            entrypoint.get_controller().push_outgoing_request(request);
        } catch (std::exception&) {}
    }
#ifdef DEBUG
    std::cout << "<- UnorderedStandingConfig::push_update_to_children(" << parent << ", sf::Vector2f)" << std::endl;
#endif
}
