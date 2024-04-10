#include "model/Character.h"
#include "Client.h"

using namespace SPlat;

int main() {
    sf::Vector2f size(50, 100);
    Model::Character character(size);
    character.setPosition(100, 100);

    Model::GameObjectModel::get_instance().create_asset(character);

    Client client; client.start();
}
