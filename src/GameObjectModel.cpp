#include "model/GameObjectModel.h"

#include <cmath>
#include <iostream>

using namespace SPlat::Model;

Asset& GameObjectModel::create_asset(Asset& asset) {
    lock.lock();

    // Get Asset ID
    size_t id = assets.size();
    while (assets.count(id) > 0) id++;
    ids.insert(id);

    // Assign ID and store
    asset.id = id;
    assets[id] = &asset;

    lock.unlock();

    return asset;
}

Asset& GameObjectModel::read_asset(size_t id) {
    lock.lock();

    if (assets.count(id) == 0) {
        lock.unlock();
        throw std::invalid_argument("No asset with id " + id);
    }
    
    Asset& asset = *assets[id];

    lock.unlock();

    return asset;
}

void GameObjectModel::validate(size_t id) {
    Asset& asset = read_asset(id);

    lock.lock();

    if (asset.standing_on != nullptr) {
        sf::Vector2f position = asset.getPosition();
        position.y += 1;
        asset.setPosition(position);
        if (!asset.getGlobalBounds().intersects(
                asset.standing_on->getGlobalBounds())) {
            asset.standing_on = nullptr;
        }
        position.y -= 1;
        asset.setPosition(position);
    }

    lock.unlock();
}

Asset& GameObjectModel::update_asset(size_t id, Asset& update) {
    lock.lock();

    if (assets.count(id) == 0) {
        lock.unlock();
        throw std::invalid_argument("No asset with id " + id);
    }

    Asset& asset = *assets[id];

    sf::Vector2f position_i = asset.getPosition();

    asset.setPosition(update.getPosition());
    asset.setSize(update.getSize());
    asset.setTexture(update.getTexture());
    asset.setTextureRect(update.getTextureRect());
    asset.velocity = update.velocity;

    sf::Vector2f position_f = asset.getPosition();

    // if position has changed, check for collision
    sf::Vector2f position_diff = position_f - position_i;
    if (fabs(position_diff.x) + fabs(position_diff.y) > 0.5) {
        check_collision(id);
    }

    lock.unlock();

    return asset;
}

Asset& GameObjectModel::delete_asset(size_t id) {
    lock.lock();

    if (assets.count(id) == 0) {
        lock.unlock();
        throw std::invalid_argument("No asset with id " + id);
    }

    Asset& asset = *assets[id];

    assets.erase(id);
    ids.erase(id);

    lock.unlock();

    return asset;
}

bool collision_checker(Asset& lhs, Asset& rhs) {
    return lhs.getGlobalBounds()
        .intersects(rhs.getGlobalBounds());
}

void GameObjectModel::check_collision(size_t id) {
    Asset * lhs = assets[id];

    if (lhs == nullptr) return;
    
    for (size_t other_id : ids) {
        if (id == other_id) continue;

        Asset * rhs = assets[other_id];
        if (rhs == nullptr) continue;

        bool collision = collision_checker(*lhs, *rhs);
        
        if (!collision) continue;

        resolve_collision(*lhs, *rhs);
        lhs->get_priority() >= rhs->get_priority()
            ? resolve_collision(*lhs, *rhs)
            : resolve_collision(*rhs, *lhs);
    }
}

void GameObjectModel::resolve_collision(Asset& move, Asset& fixed) {
    // get moving asset velocity as unit
    sf::Vector2f unit_velocity = move.velocity;
    float magnitude = sqrt(pow(unit_velocity.x, 2) + pow(unit_velocity.y, 2));
    if (fabs(magnitude) < 0.0001) return;
    unit_velocity /= magnitude;

    sf::Vector2f offset;
    {
        sf::RectangleShape cpy = move;
        float x_unit = -fabs(move.velocity.x) / move.velocity.x;
        do {
            cpy.move(x_unit, 0);
            offset.x += x_unit;
        } while (cpy.getGlobalBounds().intersects(fixed.getGlobalBounds()));
    }{
        sf::RectangleShape cpy = move;
        float y_unit = -fabs(move.velocity.y) / move.velocity.y;
        do {
            cpy.move(0, y_unit);
            offset.y += y_unit;
        } while (cpy.getGlobalBounds().intersects(fixed.getGlobalBounds()));
    }
    // check for nan
    if (!std::isnan(offset.x) && (std::isnan(offset.y) || fabs(offset.x) < fabs(offset.y))) {
        move.move(offset.x, 0);
    } else if (!std::isnan(offset.y) && (std::isnan(offset.x) || fabs(offset.y) < fabs(offset.x))) {
        move.move(0, offset.y);
    }

    // check if move on fixed object now
    {
        sf::RectangleShape cpy = move;
        cpy.move(0, 1);
        if (cpy.getGlobalBounds().intersects(fixed.getGlobalBounds())) {
            move.standing_on = &fixed;
            move.velocity.y = 0;
        }
    }

    // check if collision was with something above; if so, y vel becomes 0
    {
        sf::RectangleShape cpy = move;
        cpy.move(0, -1);
        if (cpy.getGlobalBounds().intersects(fixed.getGlobalBounds()))
            move.velocity.y = 0;
    }
}