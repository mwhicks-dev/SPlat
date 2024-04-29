#include "model/MovingPlatformCollisionHandler.h"

#include <cmath>

using namespace SPlat::Model;

AssetProperties& MovingPlatformCollisionHandler::get_asset_properties() {
    m.lock();
    AssetProperties& local = asset;
    m.unlock();

    return local;
}

MovingProperties& MovingPlatformCollisionHandler::get_moving_properties() {
    m.lock();
    MovingProperties& local = moving;
    m.unlock();

    return local;
}

MovingPlatformProperties& MovingPlatformCollisionHandler::get_moving_platform_properties() {
    m.lock();
    MovingPlatformProperties& local = moving_platform;
    m.unlock();

    return local;
}

void MovingPlatformCollisionHandler::resolve_collision(AssetProperties& other) {
    AssetProperties& self = get_asset_properties();

    // find unit velocity
    sf::Vector2f unit_velocity;
    {
        sf::Vector2f curr_velocity = get_moving_properties().get_velocity();
        float magnitude = sqrt(pow(curr_velocity.x, 2) + pow(curr_velocity.y, 2));
        sf::Vector2f unit_velocity = curr_velocity / magnitude;
    }


    // move other forward in respective direction until no more collision
    bool condition = get_asset_properties().get_collision_priority() 
        < other.get_collision_priority();
    sf::RectangleShape self_cpy = get_asset_properties().get_rectangle_shape();
    sf::RectangleShape other_cpy = other.get_rectangle_shape();
    while (self_cpy.getGlobalBounds().intersects(other_cpy.getGlobalBounds())) {
        if (condition) other_cpy.move(unit_velocity);
        else self_cpy.move(-unit_velocity);
    }
    
    // update
    if (condition) {
        other.set_position(other_cpy.getPosition());
    } else {
        self.set_position(self_cpy.getPosition());
    }
}
