#include <gtest/gtest.h>

#include "Serialization.h"
#include "cereal/archives/json.hpp"

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