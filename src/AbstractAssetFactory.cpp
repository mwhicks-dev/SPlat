#include "model/AbstractAssetFactory.h"
#include "model/GameObjectModel.h"

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

    sf::RectangleShape curr_rectangle = curr.get_rectangle_shape(),
                       other_rectangle = other.get_rectangle_shape();
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
#ifdef DEBUG
    std::cout << "<- AbstractAssetFactory::DefaultCollisionHandler::resolve_collision(AssetProperties&)" << std::endl; 
#endif
}
