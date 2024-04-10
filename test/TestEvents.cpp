#include <gtest/gtest.h>

#include "KeyEvents.h"

TEST(EventTest, KeyEvents) {
    // Pick arbitrary key -- should pass for any
    sf::Keyboard::Key k(sf::Keyboard::Key::Q);

    // Assert that key is not held
    ASSERT_FALSE(SPlat::KeyEvent::is_key_held(k));

    // Create and (manually) dispatch a key press event
    SPlat::KeyPressEvent press(k);
    SPlat::Event::handlers[press.type](press.args);

    // Assert that key is now held
    ASSERT_TRUE(SPlat::KeyEvent::is_key_held(k));

    // Create and (manually) dispatch a key release event
    SPlat::KeyReleaseEvent release(k);
    SPlat::Event::handlers[press.type](press.args);

    // Assert that key is not held
    ASSERT_FALSE(SPlat::KeyEvent::is_key_held(k));

    // Double dispatch key press
    SPlat::Event::handlers[press.type](press.args);
    SPlat::Event::handlers[press.type](press.args);

    // Assert that key is now held
    ASSERT_TRUE(SPlat::KeyEvent::is_key_held(k));

    // Dispatch key release
    SPlat::Event::handlers[press.type](press.args);

    // Assert that key is not held
    ASSERT_FALSE(SPlat::KeyEvent::is_key_held(k));

    // Dispatch key release again
    SPlat::Event::handlers[press.type](press.args);

    // Assert that key is not held
    ASSERT_FALSE(SPlat::KeyEvent::is_key_held(k));
}