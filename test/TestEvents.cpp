#include <gtest/gtest.h>
#include <iostream>
#include "events/KeyEvents.h"

using namespace SPlat;

TEST(EventTest, KeyEvents) {
    Events::Event::handlers[Events::KeyPressEvent::KEY_PRESS_EVENT_TAG] = Events::KeyPressEvent::handler;
    Events::Event::handlers[Events::KeyReleaseEvent::KEY_RELEASE_EVENT_TAG] = Events::KeyReleaseEvent::handler;

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