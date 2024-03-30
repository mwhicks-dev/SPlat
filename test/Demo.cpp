#include "Character.h"
#include "Client.h"

int main() {
    sf::Vector2f size(50, 100);
    SPlat::Character character(size);
    character.setPosition(100, 100);

    SPlat::GameObjectModel::get_instance().create_asset(character);

    SPlat::Client client; client.start();
}
