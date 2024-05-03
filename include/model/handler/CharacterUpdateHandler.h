#ifndef SPlat_CharacterUpdateHandler_h
#define SPlat_CharacterUpdateHandler_h

#include "model/AssetProperties.h"
#include "model/MovingProperties.h"
#include "model/CharacterProperties.h"
#include "model/handler/UpdateHandler.h"

namespace SPlat {

    namespace Model {

        class CharacterUpdateHandler : public UpdateHandler {

            std::mutex m;

            AssetProperties& asset;

            MovingProperties& moving;

            CharacterProperties& character;

            AssetProperties& get_asset_properties();

            MovingProperties& get_moving_properties();

            CharacterProperties& get_character_properties();

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