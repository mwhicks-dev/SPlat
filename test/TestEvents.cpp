#include <gtest/gtest.h>
#include <iostream>
#include "KeyEvents.h"

TEST(EventTest, KeyEvents) {
    SPlat::Event::handlers[SPlat::KeyPressEvent::KEY_PRESS_EVENT_TAG] = SPlat::KeyPressEvent::handler;
    SPlat::Event::handlers[SPlat::KeyReleaseEvent::KEY_RELEASE_EVENT_TAG] = SPlat::KeyReleaseEvent::handler;

    // Pick arbitrary key -- should pass for any
    sf::Keyboard::Key k(sf::Keyboard::Key::Q);

    // Assert that key is not held
    ASSERT_FALSE(SPlat::KeyEvent::is_key_held(k));

    // Create and (manually) dispatch a key press event
    SPlat::KeyPressEvent press(k);
    press.dispatch();

    // Assert that key is now held
    ASSERT_TRUE(SPlat::KeyEvent::is_key_held(k));

    // Create and (manually) dispatch a key release event
    SPlat::KeyReleaseEvent release(k);
    release.dispatch();

    // Assert that key is not held
    ASSERT_FALSE(SPlat::KeyEvent::is_key_held(k));

    // Double dispatch key press
    press.dispatch(); press.dispatch();

    // Assert that key is now held
    ASSERT_TRUE(SPlat::KeyEvent::is_key_held(k));

    // Dispatch key release
    release.dispatch();

    // Assert that key is not held
    ASSERT_FALSE(SPlat::KeyEvent::is_key_held(k));

    // Dispatch key release again
    release.dispatch();

    // Assert that key is not held
    ASSERT_FALSE(SPlat::KeyEvent::is_key_held(k));
}