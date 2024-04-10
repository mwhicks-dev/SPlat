#include <gtest/gtest.h>
#include <iostream>
#include "events/KeyEvents.h"

TEST(EventTest, KeyEvents) {
    SPlat::Events::Event::handlers[SPlat::Events::KeyPressEvent::KEY_PRESS_EVENT_TAG] = SPlat::Events::KeyPressEvent::handler;
    SPlat::Events::Event::handlers[SPlat::Events::KeyReleaseEvent::KEY_RELEASE_EVENT_TAG] = SPlat::Events::KeyReleaseEvent::handler;

    // Pick arbitrary key -- should pass for any
    sf::Keyboard::Key k(sf::Keyboard::Key::Q);

    // Assert that key is not held
    ASSERT_FALSE(SPlat::Events::KeyEvent::is_key_held(k));

    // Create and (manually) dispatch a key press event
    SPlat::Events::KeyPressEvent press(k);
    press.dispatch();

    // Assert that key is now held
    ASSERT_TRUE(SPlat::Events::KeyEvent::is_key_held(k));

    // Create and (manually) dispatch a key release event
    SPlat::Events::KeyReleaseEvent release(k);
    release.dispatch();

    // Assert that key is not held
    ASSERT_FALSE(SPlat::Events::KeyEvent::is_key_held(k));

    // Double dispatch key press
    press.dispatch(); press.dispatch();

    // Assert that key is now held
    ASSERT_TRUE(SPlat::Events::KeyEvent::is_key_held(k));

    // Dispatch key release
    release.dispatch();

    // Assert that key is not held
    ASSERT_FALSE(SPlat::Events::KeyEvent::is_key_held(k));

    // Dispatch key release again
    release.dispatch();

    // Assert that key is not held
    ASSERT_FALSE(SPlat::Events::KeyEvent::is_key_held(k));
}