#include "Client.h"
#include "model/Character.h"
#include "events/CreateAssetEvent.h"

using namespace SPlat;

int main() {
    // Add create asset event handler to set
    Events::Event::handlers[Events::CreateAssetEvent::CREATE_ASSET_EVENT_TAG] 
        = Events::CreateAssetEvent::handler;

    // Create asset with dispatched event
    SPlat::Events::CreateAssetEvent({
        sf::Vector2f(100, 100), // position
        sf::Vector2f(50, 100),  // size
        SPlat::Model::Character::TYPE  // type
    }).dispatch();

    Client client; client.start();
}
