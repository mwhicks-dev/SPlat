#include "Config.h"
#include "model/GameObjectModel.h"
#include "model/MovingPlatformFactory.h"
#include "model/MovingPlatform.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

Asset& MovingPlatformFactory::create_asset(AssetProperties& properties) {
    // create new moving and movingplatform properties
    MovingProperties moving_properties(sf::Vector2f(0, 0), 
        Config::get_instance().get_timing_config().get_display_timeline().get_time());
    MovingPlatformProperties moving_platform_properties(-1, {});

    MovingPlatform * mplat = new MovingPlatform(
        *new AssetProperties(properties), 
        *new MovingProperties (moving_properties), 
        *new MovingPlatformProperties(moving_platform_properties));

    return GameObjectModel::get_instance().create_asset(*mplat);
}
