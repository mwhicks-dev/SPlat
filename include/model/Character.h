#ifndef SPLAT_CHARACTER
#define SPLAT_CHARACTER

#include "model/Moving.h"
#include "model/CharacterProperties.h"

namespace SPlat {

    namespace Model {

        class Character : public Moving {

            CharacterProperties properties;

        public:

            Character(CharacterProperties& properties, 
                    CollisionHandler& collision_handler,
                    UpdateHandler& update_handler) : Moving(properties, 
                    collision_handler, update_handler), 
                    properties(properties) {};
            
            AssetProperties& get_asset_properties() override { return properties; }
            
            MovingProperties& get_moving_properties() override { return properties; }

        };

    }

}

#endif