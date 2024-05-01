#include "model/handler/MovingPlatformCollisionHandler.h"

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
#ifdef DEBUG
    std::cout << "-> MovingPlatformCollisionHandler::resolve_collision(AssetProperties&)" << std::endl;
#endif
    AssetProperties& self = get_asset_properties();

    sf::RectangleShape self_rect = self.get_rectangle_shape();
    sf::RectangleShape other_rect = other.get_rectangle_shape();

    if (!self_rect.getGlobalBounds().intersects(other_rect.getGlobalBounds())) return;

    // find unit velocity
    sf::Vector2f unit_velocity;
    {
        sf::Vector2f curr_velocity = get_moving_properties().get_velocity();
        float magnitude = sqrt(pow(curr_velocity.x, 2) + pow(curr_velocity.y, 2));
        unit_velocity = curr_velocity / magnitude;
    }


    // move other forward in respective direction until no more collision
    bool condition = self.get_collision_priority() 
        < other.get_collision_priority();
    while (self_rect.getGlobalBounds().intersects(other_rect.getGlobalBounds())) {
        if (condition) other_rect.move(unit_velocity);
        else self_rect.move(-unit_velocity);
    }
    
    // update
    if (condition) {
        other.set_position(other_rect.getPosition());
    } else {
        self.set_position(self_rect.getPosition());
    }
#ifdef DEBUG
    std::cout << "<- MovingPlatformCollisionHandler::resolve_collision" << std::endl;
#endif
}
