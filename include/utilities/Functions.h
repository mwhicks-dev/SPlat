#ifndef SPLAT_FUNCTIONS_H
#define SPLAT_FUNCTIONS_H

#include "model/Character.h"
#include "model/Platform.h"
#include "model/MovingPlatform.h"
#include "model/GameObjectModel.h"
#include "events/AssetEvents.h"
#include "events/ControlAssetEvent.h"
#include "events/CreateAssetEvent.h"
#include "events/CharacterEvents.h"
#include "events/CreateControlAssetEvent.h"
#include "events/KeyEvents.h"
#include "events/MovingPlatformEvents.h"
#include "events/TickEvent.h"

namespace SPlat {

    namespace Utilities {

        /// @brief deserializes AssetProperties string
        /// @param serialized serialized AssetProperties
        static SPlat::Model::Asset& deserialize_asset(std::string serialized) {
            // deserialize args from string
            SPlat::Model::AssetProperties args;
            std::stringstream ss; ss << serialized;
            {
                cereal::JSONInputArchive iar(ss);
                iar(args);
            }

            // create new asset using GOM
            SPlat::Model::Asset * created_asset;
            if (args.type == SPlat::Model::Character::TYPE) {
                created_asset = new SPlat::Model::Character(args.size);
            } else if (args.type == SPlat::Model::Platform::TYPE) {
                created_asset = new SPlat::Model::Platform(args.size);
            } else if (args.type == SPlat::Model::MovingPlatform::TYPE) {
                created_asset = new SPlat::Model::MovingPlatform(args.size);
            } else {
                throw std::domain_error("No such asset type " + args.type);
            }

            created_asset->setPosition(args.position);
            return *created_asset;
        }

        /// @brief sets all SPlat lib event handlers
        static void set_default_handlers() {
            SPlat::Events::Event::handlers[SPlat::Events::AddPositionEvent::TYPE] =
                SPlat::Events::AddPositionEvent::handler;

            SPlat::Events::Event::handlers[SPlat::Events::AddVelocityEvent::TYPE] =
                SPlat::Events::AddVelocityEvent::handler;

            SPlat::Events::Event::handlers[SPlat::Events::ControlAssetEvent::TYPE] =
                SPlat::Events::ControlAssetEvent::handler;
            
            SPlat::Events::Event::handlers[SPlat::Events::CreateAssetEvent::TYPE] =
                SPlat::Events::CreateAssetEvent::handler;
            
            SPlat::Events::Event::handlers[SPlat::Events::CreateCharacterEvent::TYPE] =
                SPlat::Events::CreateCharacterEvent::handler;
            
            SPlat::Events::Event::handlers[SPlat::Events::CreateControlAssetEvent::TYPE] =
                SPlat::Events::CreateControlAssetEvent::handler;
            
            SPlat::Events::Event::handlers[SPlat::Events::CreateControlCharacterEvent::TYPE] =
                SPlat::Events::CreateControlCharacterEvent::handler;
            
            SPlat::Events::Event::handlers[SPlat::Events::CreateMovingPlatformEvent::TYPE] =
                SPlat::Events::CreateMovingPlatformEvent::handler;
            
            SPlat::Events::Event::handlers[SPlat::Events::KeyPressEvent::TYPE] =
                SPlat::Events::KeyPressEvent::handler;
            
            SPlat::Events::Event::handlers[SPlat::Events::KeyReleaseEvent::TYPE] =
                SPlat::Events::KeyReleaseEvent::handler;

            SPlat::Events::Event::handlers[SPlat::Events::TickEvent::TYPE] = 
                SPlat::Events::TickEvent::handler;
        }

    }

}

#endif