#include "model/CharacterFactory.h"
#include "model/Character.h"
#include "model/GameObjectModel.h"
#include "Config.h"

#include <exception>
#include <limits>
#include <cmath>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

Asset& CharacterFactory::create_asset(AssetProperties& properties) {
    Character * character = new Character(
        (CharacterProperties&) properties,
        *new CharacterFactory::DefaultCollisionHandler((CharacterProperties*) &properties),
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

    CharacterProperties& properties = (CharacterProperties&) *get_properties();
    float dt = static_cast<float>(curr - properties.get_last_updated());

    if (properties.get_standing_on() == nullptr) {
        SPlat::EnvironmentInterface& env = SPlat::Config::get_instance().get_environment();
        sf::Vector2f velocity_modifier(0, env.get_unit() * 9.8 * dt);
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

    AbstractMovingFactory::DefaultUpdateHandler initial;
    initial.set_properties(get_properties());
    initial.update(curr);
#ifdef DEBUG
    std::cout << "<- CharacterFactory::DefaultUpdateHandler::update" << std::endl;
#endif
}

void CharacterFactory::DefaultCollisionHandler::resolve_collision(AssetProperties& other) {
#ifdef DEBUG
    std::cout << "-> CharacterFactory::DefaultCollisionHandler::resolve_collision(AssetProperties&)" << std::endl;
#endif
    if (!get_properties()->get_rectangle_shape().getGlobalBounds().intersects(other.get_rectangle_shape().getGlobalBounds())) return;

    CharacterProperties* properties = get_character_properties();
    
    if (other.get_collision_priority() <= properties->get_collision_priority()) {
        sf::Vector2f resolutions[2] = {};
        {
            sf::RectangleShape tmp = properties->get_rectangle_shape();
            if (fabs(properties->get_velocity().x) < 0.001) {
                resolutions[0].x = std::numeric_limits<float>::max();
            } else {
                sf::Vector2f unit(-properties->get_velocity().x, 0);
                while (tmp.getGlobalBounds().intersects(other.get_rectangle_shape().getGlobalBounds()))
                    tmp.move(unit);
                resolutions[0].x = tmp.getPosition().x - properties->get_rectangle_shape().getPosition().x;
            }
        }
        {
            sf::RectangleShape tmp = properties->get_rectangle_shape();
            if (fabs(properties->get_velocity().y) < 0.001) {
                resolutions[1].y = std::numeric_limits<float>::max();
            } else {
                sf::Vector2f unit(-properties->get_velocity().y, 0);
                while (tmp.getGlobalBounds().intersects(other.get_rectangle_shape().getGlobalBounds()))
                    tmp.move(unit);
                resolutions[1].y = tmp.getPosition().y - properties->get_rectangle_shape().getPosition().y;
            }
        }
        sf::Vector2f resolution;
        if (fabs(resolutions[0].x) + fabs(resolutions[0].y) 
                <= fabs(resolutions[1].x) + fabs(resolutions[1].y)) {
            resolution = resolutions[0];
        } else {
            resolution = resolutions[1];
        }
        properties->set_position(properties->get_position() + resolution);
    } else {
        other.set_position(other.get_position() + properties->get_velocity() 
            / static_cast<float>(Config::get_instance().get_environment()
            .get_framerate_limit()));
    }

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