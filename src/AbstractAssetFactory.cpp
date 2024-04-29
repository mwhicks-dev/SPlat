#include "model/AbstractAssetFactory.h"
#include "model/GameObjectModel.h"

#include <cmath>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

Asset& AbstractAssetFactory::read_asset(size_t id) {
    return GameObjectModel::get_instance().read_asset(id);
}

Asset& AbstractAssetFactory::delete_asset(size_t id) {
    return GameObjectModel::get_instance().delete_asset(id);
}

void AbstractAssetFactory::DefaultCollisionHandler
        ::resolve_collision(AssetProperties& other) {
#ifdef DEBUG
    std::cout << "-> AbstractAssetFactory::DefaultCollisionHandler::resolve_collision(AssetProperties&)" << std::endl; 
#endif
    if (this->get_properties() == nullptr)
        throw std::logic_error("Collision handler has no defined properties");
    
    AssetProperties& curr = *this->get_properties();
    if (other.get_collision_priority() <= get_properties()->get_collision_priority()) return;

    sf::RectangleShape curr_rectangle = curr.get_rectangle_shape(),
                       other_rectangle = other.get_rectangle_shape();
    if (!curr_rectangle.getGlobalBounds().intersects(other_rectangle.getGlobalBounds())) return;
    // find shortest resolution for other s.t. collision resolved
    sf::Vector2f resolutions[4] = {};
    {
        sf::RectangleShape tmp = other_rectangle;
        while (tmp.getGlobalBounds().intersects(
                curr_rectangle.getGlobalBounds())) {
            tmp.move(sf::Vector2f(0, 1));
        }
        resolutions[0] = tmp.getPosition() - other_rectangle.getPosition();
    }
    {
        sf::RectangleShape tmp = other_rectangle;
        while (tmp.getGlobalBounds().intersects(
                curr_rectangle.getGlobalBounds())) {
            tmp.move(sf::Vector2f(0, -1));
        }
        resolutions[1] = tmp.getPosition() - other_rectangle.getPosition();
    }
    {
        sf::RectangleShape tmp = other_rectangle;
        while (tmp.getGlobalBounds().intersects(
                curr_rectangle.getGlobalBounds())) {
            tmp.move(sf::Vector2f(-1, 0));
        }
        resolutions[2] = tmp.getPosition() - other_rectangle.getPosition();
    }
    {
        sf::RectangleShape tmp = other_rectangle;
        while (tmp.getGlobalBounds().intersects(
                curr_rectangle.getGlobalBounds())) {
            tmp.move(sf::Vector2f(1, 0));
        }
        resolutions[3] = tmp.getPosition() - other_rectangle.getPosition();
    }

    size_t min_index = 0;
    for (size_t i = 1; i < sizeof(resolutions) / sizeof(sf::Vector2f); i++) {
        if (fabs(resolutions[i].x) + fabs(resolutions[i].y) 
                < fabs(resolutions[min_index].x) + fabs(resolutions[min_index].y))
            min_index = i;
    }

    other.set_position(other.get_position() + resolutions[min_index]);
#ifdef DEBUG
    std::cout << "<- AbstractAssetFactory::DefaultCollisionHandler::resolve_collision(AssetProperties&)" << std::endl; 
#endif
}
