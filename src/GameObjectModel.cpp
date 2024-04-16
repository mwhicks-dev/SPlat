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

Asset& GameObjectModel::update_asset(size_t id, Asset& update) {
    lock.lock();

    if (assets.count(id) == 0) {
        lock.unlock();
        throw std::invalid_argument("No asset with id " + id);
    }

    Asset& asset = *assets[id];

    sf::Vector2f position_i = asset.getPosition();

    asset.setPosition(update.getPosition());
    asset.setTexture(update.getTexture());
    asset.setTextureRect(update.getTextureRect());

    // NOTE: update velocity is ADDED to original
    asset.velocity += update.velocity;

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
    std::cout << "Collision detected!!!" << std::endl;
    std::cout << "Position: " << move.getPosition().x << ", " << move.getPosition().y << std::endl;

    // get moving asset velocity as unit
    sf::Vector2f unit_velocity = move.velocity;
    float magnitude = sqrt(pow(unit_velocity.x, 2) + pow(unit_velocity.y, 2));
    if (fabs(magnitude) < 0.0001) return;
    unit_velocity /= magnitude;

    // subtract unit velocities until no more collision
    sf::Vector2f position = move.getPosition();
    do {
        position -= unit_velocity;
        move.setPosition(position);
    } while (collision_checker(move, fixed));

    std::cout << "Position: " << move.getPosition().x << ", " << move.getPosition().y << std::endl;

    // check if move on fixed object now
    position.y += 1;
    move.setPosition(position);
    if (collision_checker(move, fixed)) {
        move.standing_on = &fixed;
        move.velocity.y = 0;
    }
    position.y -= 1;
    move.setPosition(position);
}