#ifndef SPlat_CharacterCollisionHandler_h
#define SPlat_CharacterCollisionHandler_h

#include "model/AssetProperties.h"
#include "model/MovingProperties.h"
#include "model/CharacterProperties.h"
#include "model/handler/CollisionHandler.h"

namespace SPlat {

    namespace Model {

        class CharacterCollisionHandler : public CollisionHandler {

            std::mutex m;

            AssetProperties& asset;

            MovingProperties& moving;

            CharacterProperties& character;

            AssetProperties& get_asset_properties();

            MovingProperties& get_moving_properties();

            CharacterProperties& get_character_properties();

        public:

            CharacterCollisionHandler(AssetProperties& asset, 
                    MovingProperties& moving, CharacterProperties& 
                    character) : asset(asset), moving(moving), 
                    character(character) {}
            
            void resolve_collision(AssetProperties&) override;

        };

    }

};

#endif