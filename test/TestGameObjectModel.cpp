#include <gtest/gtest.h>

#include "GameObjectModel.h"

TEST(GOMTest, BasicAcquisition) {
    try {
        SPlat::GameObjectModel& gom = SPlat::GameObjectModel::get_instance();
        ASSERT_TRUE(true);
    } catch (std::exception& e) {
        FAIL();
    }
}