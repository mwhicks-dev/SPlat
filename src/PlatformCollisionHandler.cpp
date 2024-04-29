#include "model/PlatformCollisionHandler.h"

#include <cmath>
#include <limits>

using namespace SPlat::Model;

AssetProperties& PlatformCollisionHandler::get_asset_properties() {
    m.lock();
    AssetProperties& local = asset_properties;
    m.unlock();

    return local;
}

void PlatformCollisionHandler::resolve_collision(AssetProperties& other) {
    // very simple -- shortest distance resolution
    sf::Vector2f units[] = { sf::Vector2f(0, 1), sf::Vector2f(0, -1), 
        sf::Vector2f(1, 0), sf::Vector2f(-1, 0) };

    sf::RectangleShape fixed = get_asset_properties().get_rectangle_shape();
    sf::Vector2f shortest(std::numeric_limits<float>::max() / 2, std::numeric_limits<float>::max() / 2);

    for (sf::Vector2f unit : units) {
        // get rectangle copy of other and move by unit until no collision
        sf::RectangleShape cpy = other.get_rectangle_shape();
        while (fixed.getGlobalBounds().intersects(cpy.getGlobalBounds()))
            cpy.move(unit);
        sf::Vector2f dP = cpy.getPosition() - other.get_position();
        
        // record if shortest
        if (fabs(dP.x) + fabs(dP.y) < fabs(shortest.x) + fabs(shortest.y)) {
            shortest = dP;
        }
    }

    other.set_position(other.get_position() + shortest);
}
