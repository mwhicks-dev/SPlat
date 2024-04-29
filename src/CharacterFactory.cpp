#include "model/CharacterFactory.h"
#include "model/Character.h"
#include "model/GameObjectModel.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

Asset& CharacterFactory::create_asset(AssetProperties& properties) {
    Character * character = new Character(
        (CharacterProperties&) properties,
        *new CharacterFactory::DefaultCollisionHandler(),
        *new CharacterFactory::DefaultUpdateHandler()
    );

    return GameObjectModel::get_instance().create_asset(*character);
}

Asset& CharacterFactory::update_asset(size_t id, AssetProperties& properties) {
    Character& character = (Character&) SPlat::Model::GameObjectModel
        ::get_instance().read_asset(id);
    MovingProperties& curr = character.get_moving_properties();
    curr.set_position(properties.get_position());
    curr.set_size(properties.get_size());
    curr.set_fill_color(properties.get_fill_color());
    try {
        MovingProperties& other = (MovingProperties&) properties;
        curr.set_velocity(other.get_velocity());
        curr.set_last_update(other.get_last_updated());
    } catch (std::exception&) {/* OK */}

    return character;
}

void CharacterFactory::DefaultUpdateHandler::update(time_t curr) {
#ifdef DEBUG
    std::cout << "-> CharacterFactory::DefaultUpdateHandler::update(" << curr << ")" << std::endl;
#endif
    AbstractMovingFactory::DefaultUpdateHandler initial;
    initial.set_properties(get_properties());
    initial.update(curr);

    CharacterProperties& properties = (CharacterProperties&) *get_properties();
    float dt = static_cast<float>(curr - properties.get_last_updated());

    if (properties.get_standing_on() == nullptr) {
        sf::Vector2f velocity_modifier(0, 490 * dt);  /// TODO: Replace 490 with constant from Runtime
        properties.set_velocity(properties.get_velocity() + velocity_modifier);
    } else {
        try {
            MovingProperties& other = (MovingProperties&) *properties
                .get_standing_on();
            properties.set_position(properties.get_position() 
                + other.get_velocity());
        } catch (std::exception&) {/* OK */}

        {
            sf::RectangleShape tmp = properties.get_rectangle_shape();
            tmp.move(0, 1);
            if (!tmp.getGlobalBounds().intersects(properties.get_standing_on()->get_rectangle_shape().getGlobalBounds())) {
                properties.set_standing_on(nullptr);
            }
        }
    }
#ifdef DEBUG
    std::cout << "<- CharacterFactory::DefaultUpdateHandler::update" << std::endl;
#endif
}

void CharacterFactory::DefaultCollisionHandler::resolve_collision(AssetProperties& other) {
#ifdef DEBUG
    std::cout << "-> CharacterFactory::DefaultCollisionHandler::resolve_collision(AssetProperties&)" << std::endl;
#endif
    CharacterProperties* properties = (CharacterProperties*) get_properties();

    AbstractAssetFactory::DefaultCollisionHandler initial;
    initial.set_properties(properties);
    initial.resolve_collision(other);

    {
        sf::RectangleShape tmp = properties->get_rectangle_shape();
        tmp.move(0, 1);
        if (tmp.getGlobalBounds().intersects(other.get_rectangle_shape()
                .getGlobalBounds())) properties->set_standing_on(&other);
        properties->set_velocity(sf::Vector2f(properties->get_velocity().x, 0));
    }
    {
        sf::RectangleShape tmp = properties->get_rectangle_shape();
        tmp.move(0, -1);
        if (tmp.getGlobalBounds().intersects(other.get_rectangle_shape()
                .getGlobalBounds())) properties->set_standing_on(&other);
        properties->set_velocity(sf::Vector2f(properties->get_velocity().x, 0));
    }
#ifdef DEBUG
    std::cout << "<- CharacterFactory::DefaultCollisionHandler::resolve_collision(AssetProperties&)" << std::endl;
#endif
}