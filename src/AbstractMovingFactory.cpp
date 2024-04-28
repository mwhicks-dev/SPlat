#include "model/AbstractMovingFactory.h"

using namespace SPlat::Model;

void AbstractMovingFactory::DefaultUpdateHandler::update(time_t curr) {
    if (this->get_properties() == nullptr)
        throw std::logic_error("Update handler has no defined properties");
    
    MovingProperties& properties = *get_properties();

    float dt = static_cast<float>(curr - properties.get_last_updated());
    sf::Vector2f position_modifier = properties.get_velocity() * dt;
    properties.set_position(properties.get_position() + position_modifier);
}