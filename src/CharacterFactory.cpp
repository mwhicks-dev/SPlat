#include "model/CharacterFactory.h"
#include "model/Character.h"
#include "model/GameObjectModel.h"

using namespace SPlat::Model;

Asset& CharacterFactory::create_asset(AssetProperties& properties) {
    Character * character = new Character(
        (MovingProperties&) properties,
        *new CharacterFactory::DefaultCollisionHandler(),
        *new CharacterFactory::DefaultUpdateHandler()
    );
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
}

void CharacterFactory::DefaultUpdateHandler::update(time_t curr) {
    if (this->get_properties() == nullptr)
        throw std::logic_error("Update handler has no defined properties");
    
    MovingProperties& properties = *get_properties();

    time_t dt = curr - properties.get_last_updated();
    sf::Vector2f update_velocity = properties.get_velocity() * static_cast<float>(dt);

    properties.set_position(properties.get_position() + update_velocity);
}