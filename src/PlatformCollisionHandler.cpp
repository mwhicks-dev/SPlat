#include "model/handler/PlatformCollisionHandler.h"

#include <cmath>
#include <limits>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

AssetProperties& PlatformCollisionHandler::get_asset_properties() {
    const std::lock_guard<std::mutex> lock(m);
    return asset_properties;
}

void PlatformCollisionHandler::resolve_collision(AssetProperties& other) {
#ifdef DEBUG
    std::cout << "-> PlatformCollisionHandler::resolve_collision(AssetProperties&)" << std::endl;
#endif
    AssetProperties& self = get_asset_properties();

    sf::RectangleShape self_rect = self.get_rectangle_shape();
    sf::RectangleShape other_rect = other.get_rectangle_shape();

    if (!self_rect.getGlobalBounds().intersects(other_rect.getGlobalBounds())) return;

    // very simple -- shortest distance resolution
    sf::Vector2f units[] = { sf::Vector2f(0, 1), sf::Vector2f(0, -1), 
        sf::Vector2f(1, 0), sf::Vector2f(-1, 0) };

    sf::Vector2f shortest(std::numeric_limits<float>::max() / 2, std::numeric_limits<float>::max() / 2);

    for (sf::Vector2f unit : units) {
        // get rectangle copy of other and move by unit until no collision
        sf::RectangleShape cpy = other_rect;
        while (self_rect.getGlobalBounds().intersects(cpy.getGlobalBounds()))
            cpy.move(unit);
        sf::Vector2f dP = cpy.getPosition() - other_rect.getPosition();
        
        // record if shortest
        if (fabs(dP.x) + fabs(dP.y) < fabs(shortest.x) + fabs(shortest.y)) {
            shortest = dP;
        }
    }

    other.set_position(other_rect.getPosition() + shortest);
#ifdef DEBUG
    std::cout << "<- PlatformCollisionHandler::resolve_collision" << std::endl;
#endif
}
