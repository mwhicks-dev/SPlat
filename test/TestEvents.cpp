#include <gtest/gtest.h>
#include <iostream>
#include "events/KeyEvents.h"
#include "events/CreateAssetEvent.h"
#include "events/ControlAssetEvent.h"
#include "events/CreateControlAssetEvent.h"
#include "events/Listener.h"
#include "model/GameObjectModel.h"
#include "model/Platform.h"

using namespace SPlat;

TEST(EventTest, KeyEvents) {
    Events::Event::handlers[Events::KeyPressEvent::TYPE] = Events::KeyPressEvent::handler;
    Events::Event::handlers[Events::KeyReleaseEvent::TYPE] = Events::KeyReleaseEvent::handler;

    // Pick arbitrary key -- should pass for any
    sf::Keyboard::Key k(sf::Keyboard::Key::Q);

    // Assert that key is not held
    ASSERT_FALSE(Events::KeyEvent::is_key_held(k));

    // Create and (manually) dispatch a key press event
    Events::KeyPressEvent press(k);
    press.dispatch();

    // Assert that key is now held
    ASSERT_TRUE(Events::KeyEvent::is_key_held(k));

    // Create and (manually) dispatch a key release event
    Events::KeyReleaseEvent release(k);
    release.dispatch();

    // Assert that key is not held
    ASSERT_FALSE(Events::KeyEvent::is_key_held(k));

    // Double dispatch key press
    press.dispatch(); press.dispatch();

    // Assert that key is now held
    ASSERT_TRUE(Events::KeyEvent::is_key_held(k));

    // Dispatch key release
    release.dispatch();

    // Assert that key is not held
    ASSERT_FALSE(Events::KeyEvent::is_key_held(k));

    // Dispatch key release again
    release.dispatch();

    // Assert that key is not held
    ASSERT_FALSE(Events::KeyEvent::is_key_held(k));
}

TEST(EventTest, CreateAssetEvent) {
    Events::Event::handlers[Events::CreateAssetEvent::TYPE]
        = Events::CreateAssetEvent::handler;
    
    // Assert that there are no assets
    ASSERT_EQ(Model::GameObjectModel::get_instance().getIds().size(), 0);

    // Dispatch create asset event
    Events::CreateAssetEvent({
        sf::Vector2f(100, 100), // position
        sf::Vector2f(50, 100),  // size
        SPlat::Model::Platform::TYPE  // type
    }).dispatch();

    // Assert that now one asset
    ASSERT_EQ(Model::GameObjectModel::get_instance().getIds().size(), 1);

    // Delete asset(s)
    for (size_t id : Model::GameObjectModel::get_instance().getIds())
        Model::GameObjectModel::get_instance().delete_asset(id);
}

TEST(EventTest, ControlAssetEvent) {
    Events::Event::handlers[Events::ControlAssetEvent::TYPE]
        = Events::ControlAssetEvent::handler;
    
    // Assert no controlled object
    ASSERT_THROW(Events::ControlAssetEvent::get_controlled_asset_id(), std::logic_error);

    // Set arbitrary ID as controlled
    Events::ControlAssetEvent(173).dispatch();

    // Assert appropriate ID controlled
    ASSERT_EQ(173, Events::ControlAssetEvent::get_controlled_asset_id());

    Events::ControlAssetEvent::reset();
}

TEST(EventTest, CreateControlAssetEvent) {
    Events::Event::handlers[Events::CreateControlAssetEvent::TYPE]
        = Events::CreateControlAssetEvent::handler;
    
    // Assert no (controlled) assets
    ASSERT_THROW(Events::ControlAssetEvent::get_controlled_asset_id(), std::logic_error);
    ASSERT_EQ(0, Model::GameObjectModel::get_instance().getIds().size());

    // Dispatch new asset, create and control
    Events::CreateControlAssetEvent({
        sf::Vector2f(200, 200),
        sf::Vector2f(50, 100),
        Model::Platform::TYPE
    }).dispatch();

    Events::ForegroundListener::get_instance().run();

    // Assert controlled asset exists
    ASSERT_EQ(1, Model::GameObjectModel::get_instance().getIds().size());
    Model::Asset &ctl = Model::GameObjectModel::get_instance().read_asset(Events::ControlAssetEvent::get_controlled_asset_id());

    // Delete asset(s)
    for (size_t id : Model::GameObjectModel::get_instance().getIds())
        Model::GameObjectModel::get_instance().delete_asset(id);
}
