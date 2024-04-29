#ifndef SPlat_CharacterCollisionHandler_h
#define SPlat_CharacterCollisionHandler_h

#include "model/AssetProperties.h"
#include "model/MovingProperties.h"
#include "model/CharacterProperties.h"
#include "model/CollisionHandler.h"

namespace SPlat {

    namespace Model {

        class CharacterCollisionHandler : public CollisionHandler {

            AssetProperties& asset;

            MovingProperties& moving;

            CharacterProperties& character;

        public:

            CharacterCollisionHandler(AssetProperties& asset, 
                    MovingProperties& moving, CharacterProperties& 
                    character) : asset(asset), moving(moving), 
                    character(character) {}
            
            void resolve_collision(AssetProperties& other) override;

        };

    }

};

#endif