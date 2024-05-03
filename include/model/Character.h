#ifndef SPLAT_CHARACTER
#define SPLAT_CHARACTER

#include "model/Moving.h"
#include "model/CharacterProperties.h"
#include "model/handler/CollisionHandler.h"
#include "model/handler/UpdateHandler.h"

namespace SPlat {

    namespace Model {

        class Character : public Moving {

            std::mutex m;

            CollisionHandler * collision_handler = nullptr;

            UpdateHandler * update_handler = nullptr;

            CharacterProperties& properties;

        public:

            Character(AssetProperties& asset_properties, MovingProperties& 
                    moving_properties, CharacterProperties& 
                    character_properties) : Moving(asset_properties, 
                    moving_properties), properties(character_properties) {}
            
            CharacterProperties& get_character_properties() {
                const std::lock_guard<std::mutex> lock(m);
                return properties;
            }

            void resolve_collision(Asset& other) override;

            void update() override;

            static constexpr int collision_priority() { return 0; }

        };

    }

}

#endif