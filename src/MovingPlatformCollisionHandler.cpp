#include "model/handler/MovingPlatformCollisionHandler.h"

#include <cmath>

using namespace SPlat::Model;

AssetProperties& MovingPlatformCollisionHandler::get_asset_properties() {
    const std::lock_guard<std::mutex> lock(m);
    return asset;
}

MovingProperties& MovingPlatformCollisionHandler::get_moving_properties() {
    const std::lock_guard<std::mutex> lock(m);
    return moving;
}

MovingPlatformProperties& MovingPlatformCollisionHandler::get_moving_platform_properties() {
    const std::lock_guard<std::mutex> lock(m);
    return moving_platform;
}

void MovingPlatformCollisionHandler::resolve_collision(AssetProperties& other) {
#ifdef DEBUG
    std::cout << "-> MovingPlatformCollisionHandler::resolve_collision(AssetProperties&)" << std::endl;
#endif
    AssetProperties& self = get_asset_properties();

    sf::RectangleShape self_rect = self.get_rectangle_shape();
    sf::RectangleShape other_rect = other.get_rectangle_shape();

    if (!self_rect.getGlobalBounds().intersects(other_rect.getGlobalBounds())) return;

    sf::Vector2f self_velocity = get_moving_properties().get_velocity();
    if (fabs(self_velocity.x) + fabs(self_velocity.y) < 0.001) return;

    // find unit velocity
    sf::Vector2f unit_velocity;
    {
        float magnitude = sqrt(pow(self_velocity.x, 2) + pow(self_velocity.y, 2));
        unit_velocity = self_velocity / magnitude;
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
