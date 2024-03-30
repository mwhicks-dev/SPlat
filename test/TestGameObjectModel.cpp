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

    SPlat::Asset& asset = *new SPlat::Character(size);
    ASSERT_EQ(0, asset.get_priority());
    delete &asset;

    asset = *new SPlat::Platform(size);
    ASSERT_EQ(-2, asset.get_priority());
    delete &asset;

    asset = *new SPlat::MovingPlatform(size);
    ASSERT_EQ(-1, asset.get_priority());
    delete &asset;
}

TEST(GOMTest, TestGOM) {
    sf::Vector2f size(50, 100);
    SPlat::Character& character = *new SPlat::Character(size);

    character = (SPlat::Character&) SPlat::GameObjectModel::get_instance().create_asset(character);
    ASSERT_EQ(character.id, 0);
    ASSERT_EQ(character.getPosition(), sf::Vector2f(0, 0));
    ASSERT_EQ(character.getSize(), sf::Vector2f(50, 100));

    SPlat::Character& character2 = *new SPlat::Character(size);
    character2 = (SPlat::Character&) SPlat::GameObjectModel::get_instance().create_asset(character2);
    ASSERT_EQ(character.id, 0);
    ASSERT_EQ(character.getPosition(), sf::Vector2f(0, 0));
    ASSERT_EQ(character.getSize(), sf::Vector2f(50, 100));
    ASSERT_EQ(character2.id, 1);
    ASSERT_EQ(character2.getPosition(), sf::Vector2f(0, 0));
    ASSERT_EQ(character2.getSize(), sf::Vector2f(50, 100));

    size += sf::Vector2f(100, 100);
    SPlat::Character& updated = *new SPlat::Character(size);
    updated.setPosition(100, 100);

    SPlat::GameObjectModel::get_instance().update_asset(character.id, updated);

    character = (SPlat::Character&) SPlat::GameObjectModel::get_instance().read_asset(character.id);

    ASSERT_EQ(character.id, 0);
    ASSERT_EQ(character.getPosition(), sf::Vector2f(100, 100));
    ASSERT_EQ(character.getSize(), sf::Vector2f(50, 100));
    ASSERT_EQ(character2.id, 1);
    ASSERT_EQ(character2.getPosition(), sf::Vector2f(0, 0));
    ASSERT_EQ(character2.getSize(), sf::Vector2f(50, 100));

    SPlat::GameObjectModel::get_instance().delete_asset(character.id);
    try {
        character2 = (SPlat::Character&) SPlat::GameObjectModel::get_instance().read_asset(character.id);
        FAIL();
    } catch (std::exception& e) {
        ASSERT_TRUE(true);
    }

    ASSERT_EQ(character.id, 0);
    ASSERT_EQ(character.getPosition(), sf::Vector2f(100, 100));
    ASSERT_EQ(character.getSize(), sf::Vector2f(50, 100));
    ASSERT_EQ(character2.id, 1);
    ASSERT_EQ(character2.getPosition(), sf::Vector2f(0, 0));
    ASSERT_EQ(character2.getSize(), sf::Vector2f(50, 100));

    delete &character;

    SPlat::Character& character3 = *new SPlat::Character(size);
    character3 = (SPlat::Character&) SPlat::GameObjectModel::get_instance().create_asset(character3);
    ASSERT_EQ(character2.id, 1);
    ASSERT_EQ(character2.getPosition(), sf::Vector2f(0, 0));
    ASSERT_EQ(character2.getSize(), sf::Vector2f(50, 100));
    ASSERT_EQ(character3.id, 2);
    ASSERT_EQ(character3.getPosition(), sf::Vector2f(0, 0));
    ASSERT_EQ(character3.getSize(), sf::Vector2f(150, 200));

    delete &character2;
    delete &character3;
}