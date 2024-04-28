#include <gtest/gtest.h>

#include "cereal/archives/json.hpp"

#include "events/Command.h"
#include "Serialization.h"

/// @brief JSON serialization of Vector2f 
TEST(SerializationTest, Vector2f) {
    // Outermost scope variables
    std::stringstream ss;
    sf::Vector2f original(6.23, 3.0001), deserialized;

    {  // Serialize original
        cereal::JSONOutputArchive oar(ss);
        oar(original);
    }

    {  // Deserialize JSON
        cereal::JSONInputArchive iar(ss);
        iar(deserialized);
    }

    ASSERT_TRUE(abs(original.x - deserialized.x) < 0.001);
    ASSERT_TRUE(abs(original.y - deserialized.y) < 0.001);
}

/// @brief JSON serialization of Event 
TEST(SerializationTest, Event) {
    std::stringstream ss;
    SPlat::Events::Command cmd = {.priority=-2, .type="event_type", .args="event_args"}, deserialized;

    {
        cereal::JSONOutputArchive oar(ss);
        oar(cmd);
    }

    {
        cereal::JSONInputArchive iar(ss);
        iar(deserialized);
    }

    ASSERT_EQ(deserialized.priority, -2);
    ASSERT_EQ(deserialized.type, "event_type");
    ASSERT_EQ(deserialized.args, "event_args");
}