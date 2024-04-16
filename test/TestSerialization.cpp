#include <gtest/gtest.h>

#include "cereal/archives/json.hpp"

#include "events/Event.h"
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
    SPlat::Events::Event event("event_type", "event_args", false), deserialized;

    {
        cereal::JSONOutputArchive oar(ss);
        oar(event);
    }

    {
        cereal::JSONInputArchive iar(ss);
        iar(deserialized);
    }

    ASSERT_EQ(deserialized.type, "event_type");
    ASSERT_EQ(deserialized.args, "event_args");
}