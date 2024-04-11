#include "Client.h"
#include "model/Character.h"
#include "utilities/Functions.h"

using namespace SPlat;

int main() {
    // Add create asset event handler to set
    Events::Event::handlers[Events::CreateAssetEvent::TYPE] 
        = Events::CreateAssetEvent::handler;

    // Create asset with dispatched event
    SPlat::Events::CreateAssetEvent({
        sf::Vector2f(100, 100), // position
        sf::Vector2f(50, 100),  // size
        SPlat::Model::Character::TYPE  // type
    }).dispatch();

    Utilities::set_default_handlers();

    Client client; client.start();
}
