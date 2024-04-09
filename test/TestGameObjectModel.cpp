#include <gtest/gtest.h>

#include "Character.h"
#include "Platform.h"
#include "MovingPlatform.h"
#include "GameObjectModel.h"

TEST(GOMTest, Acquisition) {
    try {
        SPlat::GameObjectModel& gom = SPlat::GameObjectModel::get_instance();
        ASSERT_TRUE(true);
    } catch (std::exception& e) {
        FAIL();
    }
}

TEST(GOMTest, TestAsset) {
    sf::Vector2f size(50, 100);

    SPlat::Character base1(size);
    SPlat::Asset& asset1(base1);
    ASSERT_EQ(0, asset1.get_priority());

    SPlat::Platform base2(size);
    SPlat::Asset& asset2(base2);
    ASSERT_EQ(-2, asset2.get_priority());

    SPlat::MovingPlatform base3(size);
    SPlat::Asset& asset3(base3);
    ASSERT_EQ(-1, asset3.get_priority());
}

TEST(GOMTest, TestGOM) {
    sf::Vector2f size(50, 100);
    SPlat::Character base = SPlat::Character(size);

    SPlat::Asset& character = SPlat::GameObjectModel::get_instance().create_asset(base);
    ASSERT_EQ(character.id, 0);
    ASSERT_EQ(character.getPosition(), sf::Vector2f(0, 0));
    ASSERT_EQ(character.getSize(), sf::Vector2f(50, 100));

    SPlat::Character base2(size);
    SPlat::Asset& character2 = SPlat::GameObjectModel::get_instance().create_asset(base2);
    ASSERT_EQ(character.id, 0);
    ASSERT_EQ(character.getPosition(), sf::Vector2f(0, 0));
    ASSERT_EQ(character.getSize(), sf::Vector2f(50, 100));
    ASSERT_EQ(character2.id, 1);
    ASSERT_EQ(character2.getPosition(), sf::Vector2f(0, 0));
    ASSERT_EQ(character2.getSize(), sf::Vector2f(50, 100));

    size += sf::Vector2f(100, 100);
    SPlat::Character updated(size);
    updated.setPosition(100, 100);

    SPlat::GameObjectModel::get_instance().update_asset(character.id, updated);

    character = SPlat::GameObjectModel::get_instance().read_asset(character.id);

    ASSERT_EQ(character.id, 0);
    ASSERT_EQ(character.getPosition(), sf::Vector2f(100, 100));
    ASSERT_EQ(character.getSize(), sf::Vector2f(50, 100));
    ASSERT_EQ(character2.id, 1);
    ASSERT_EQ(character2.getPosition(), sf::Vector2f(0, 0));
    ASSERT_EQ(character2.getSize(), sf::Vector2f(50, 100));

    SPlat::GameObjectModel::get_instance().delete_asset(character.id);
    try {
        character2 = SPlat::GameObjectModel::get_instance().read_asset(character.id);
        FAIL();
    } catch (std::exception& e) {
        ASSERT_TRUE(true);
    }

    ASSERT_EQ(character2.id, 1);
    ASSERT_EQ(character2.getPosition(), sf::Vector2f(0, 0));
    ASSERT_EQ(character2.getSize(), sf::Vector2f(50, 100));

    SPlat::Character base3(size);
    SPlat::Asset& character3 = SPlat::GameObjectModel::get_instance().create_asset(character3);
    ASSERT_EQ(character2.id, 1);
    ASSERT_EQ(character2.getPosition(), sf::Vector2f(0, 0));
    ASSERT_EQ(character2.getSize(), sf::Vector2f(50, 100));
    ASSERT_EQ(character3.id, 2);
    ASSERT_EQ(character3.getPosition(), sf::Vector2f(0, 0));
    ASSERT_EQ(character3.getSize(), sf::Vector2f(150, 200));
}