#include <gtest/gtest.h>

#include "model/Character.h"
#include "model/Platform.h"
#include "model/AssetFactory.h"

using namespace SPlat::Model;

TEST(AssetFactoryTest, TestFunc) {
    sf::Vector2f size(50, 100);
    Character c(size);

    c = AssetFactory<Character>::create_asset(c);
    ASSERT_EQ(0, c.id);
    ASSERT_EQ(0, c.getPosition().x);
    ASSERT_EQ(0, c.getPosition().y);
    ASSERT_EQ(50, c.getSize().x);
    ASSERT_EQ(100, c.getSize().y);

    Character cr = AssetFactory<Character>::read_asset(0);
    ASSERT_EQ(0, cr.id);
    ASSERT_EQ(0, cr.getPosition().x);
    ASSERT_EQ(0, cr.getPosition().y);
    ASSERT_EQ(50, cr.getSize().x);
    ASSERT_EQ(100, cr.getSize().y);

    ASSERT_ANY_THROW(AssetFactory<Character>::read_asset(1));

    size = sf::Vector2f(75, 100);
    Character update(size);
    Character cu = AssetFactory<Character>::update_asset(0, update);
    ASSERT_EQ(0, cu.id);
    ASSERT_EQ(0, cu.getPosition().x);
    ASSERT_EQ(0, cu.getPosition().y);
    ASSERT_EQ(75, cu.getSize().x);
    ASSERT_EQ(100, cu.getSize().y);

    ASSERT_ANY_THROW(AssetFactory<Character>::update_asset(1, update));

    cr = AssetFactory<Character>::read_asset(0);
    ASSERT_EQ(0, cr.id);
    ASSERT_EQ(0, cr.getPosition().x);
    ASSERT_EQ(0, cr.getPosition().y);
    ASSERT_EQ(75, cr.getSize().x);
    ASSERT_EQ(100, cr.getSize().y);


    Character cd = AssetFactory<Character>::delete_asset(0);
    ASSERT_EQ(0, cd.id);
    ASSERT_EQ(0, cd.getPosition().x);
    ASSERT_EQ(0, cd.getPosition().y);
    ASSERT_EQ(75, cd.getSize().x);
    ASSERT_EQ(100, cd.getSize().y);

    ASSERT_ANY_THROW(AssetFactory<Character>::delete_asset(0));

    ASSERT_ANY_THROW(AssetFactory<Character>::read_asset(0));

    // ASSERT_ANY_THROW(AssetFactory<size_t>::create_asset(0));  // this isn't caught for some reason
}
