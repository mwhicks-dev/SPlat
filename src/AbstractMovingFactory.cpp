#include "model/AbstractMovingFactory.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

void AbstractMovingFactory::DefaultUpdateHandler::update(time_t curr) {
#ifdef DEBUG
    std::cout << "-> AbstractMovingFactory::DefaultUpdateHandler::update(" << curr << ")" << std::endl;
#endif
    if (this->get_properties() == nullptr) {
        throw std::logic_error("Update handler has no defined properties");
#ifdef DEBUG
        std::cout << "<- AbstractMovingFactory::DefaultUpdateHandler::update" << std::endl;
#endif
    }
    
    MovingProperties& properties = *get_properties();

    float dt = static_cast<float>(curr - properties.get_last_updated());

    sf::Vector2f position_modifier = properties.get_velocity() * dt;
    properties.set_position(properties.get_position() + position_modifier);
#ifdef DEBUG
    std::cout << "<- AbstractMovingFactory::DefaultUpdateHandler::update" << std::endl;
#endif
}