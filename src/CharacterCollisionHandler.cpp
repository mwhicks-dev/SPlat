#include "model/handler/CharacterCollisionHandler.h"
#include "Entrypoint.h"

#include <cmath>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

AssetProperties& CharacterCollisionHandler::get_asset_properties() {
    const std::lock_guard<std::mutex> lock(m);
    return asset;
}

MovingProperties& CharacterCollisionHandler::get_moving_properties() {
    const std::lock_guard<std::mutex> lock(m);
    return moving;
}

CharacterProperties& CharacterCollisionHandler::get_character_properties() {
    const std::lock_guard<std::mutex> lock(m);
    return character;
}

void CharacterCollisionHandler::resolve_collision(AssetProperties& other) {
#ifdef DEBUG
    std::cout << "-> CharacterCollisionHandler::resolve_collision(AssetProperties&)" << std::endl;
#endif
    EnvironmentInterface& env = Entrypoint::get_instance().get_config().get_environment();
    AssetProperties& self = get_asset_properties();

    sf::RectangleShape self_rect = self.get_rectangle_shape();
    sf::RectangleShape other_rect = other.get_rectangle_shape();

    if (!self_rect.getGlobalBounds().intersects(other_rect.getGlobalBounds())) return;

    sf::Vector2f unit_velocity;
    {
        sf::Vector2f self_velocity = get_moving_properties().get_velocity();
        float magnitude = sqrt(pow(self_velocity.x, 2) + pow(self_velocity.y, 2));
        unit_velocity = self_velocity / magnitude;
    }
    if (other.get_collision_priority() <= self.get_collision_priority()) {

        // shortest component distance opposite velocity
        unit_velocity = -unit_velocity;
        sf::Vector2f shortest;
        sf::RectangleShape tmp;
        if (fabs(unit_velocity.x) > fabs(unit_velocity.y)) {
            // set unit velocity to component units
            unit_velocity.x = unit_velocity.x / fabs(unit_velocity.x);
            unit_velocity.y = unit_velocity.y / fabs(unit_velocity.y);

            if (std::isnan(unit_velocity.x)) unit_velocity.x = 0;
            if (std::isnan(unit_velocity.y)) unit_velocity.y = 0;

            // try x first
            tmp = self_rect;
            while (tmp.getGlobalBounds().intersects(other_rect.getGlobalBounds()))
                tmp.move(unit_velocity.x, 0);
            shortest.x = tmp.getPosition().x - self_rect.getPosition().x;

            // try y second
            tmp = self_rect;
            tmp.move(0, fabs(shortest.x) * unit_velocity.y);
            if (!tmp.getGlobalBounds().intersects(other_rect.getGlobalBounds())) {
                shortest.x = 0; shortest.y = unit_velocity.y;
            } else {
                shortest.x = unit_velocity.x;
            }
        } else {
            // set unit velocity to component units
            unit_velocity.x = unit_velocity.x / fabs(unit_velocity.x);
            unit_velocity.y = unit_velocity.y / fabs(unit_velocity.y);

            if (std::isnan(unit_velocity.x)) unit_velocity.x = 0;
            if (std::isnan(unit_velocity.y)) unit_velocity.y = 0;

            // try y first
            tmp = self_rect;
            while (tmp.getGlobalBounds().intersects(other_rect.getGlobalBounds()))
                tmp.move(0, unit_velocity.y);
            shortest.y = tmp.getPosition().y - self_rect.getPosition().y;

            // try x second
            tmp = self_rect;
            tmp.move(0, fabs(shortest.y) * unit_velocity.x);
            if (!tmp.getGlobalBounds().intersects(other_rect.getGlobalBounds())) {
                shortest.y = 0; shortest.x = unit_velocity.x;
            } else {
                shortest.y = unit_velocity.y;
            }
        }

        // move
        while (self_rect.getGlobalBounds().intersects(other_rect.getGlobalBounds()))
            self_rect.move(shortest);
        self.set_position(self_rect.getPosition());

        // check if collision was vertical or landing
        bool vertical = false, landing = false;
        {
            sf::RectangleShape tmp = self.get_rectangle_shape();
            tmp.move(0, -1);
            if (tmp.getGlobalBounds().intersects(other_rect.getGlobalBounds()))
                vertical = true;
        }
        {
            sf::RectangleShape tmp = self.get_rectangle_shape();
            tmp.move(0, 1);
            if (tmp.getGlobalBounds().intersects(other_rect.getGlobalBounds()))
                { vertical = true; landing = true; }
        }

        if (vertical) {
            MovingProperties& self_moving = get_moving_properties();
            self_moving.set_velocity(sf::Vector2f(self_moving
                .get_velocity().x, 0));
            if (landing) {
                get_character_properties().set_standing_on(&other);
                env.get_standing_config().push_child(other.get_id(), self.get_id());
            }
        }
    } else {
        // shortest distance velocity
        while (self_rect.getGlobalBounds().intersects(other_rect.getGlobalBounds()))
            other_rect.move(unit_velocity);
        other.set_position(other_rect.getPosition());
    }
#ifdef DEBUG
    std::cout << "<- CharacterCollisionHandler::resolve_collision" << std::endl;
#endif
}