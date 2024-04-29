#ifndef SPlat_CharacterUpdateHandler_h
#define SPlat_CharacterUpdateHandler_h

#include "model/AssetProperties.h"
#include "model/MovingProperties.h"
#include "model/CharacterProperties.h"
#include "model/UpdateHandler.h"

namespace SPlat {

    namespace Model {

        class CharacterUpdateHandler : public UpdateHandler {

            AssetProperties& asset;

            MovingProperties& moving;

            CharacterProperties& character;

        public:

            CharacterUpdateHandler(AssetProperties& asset, 
                    MovingProperties& moving, CharacterProperties& 
                    character) : asset(asset), moving(moving), 
                    character(character) {}
            
            void update() override;

        };

    }

};

#endif