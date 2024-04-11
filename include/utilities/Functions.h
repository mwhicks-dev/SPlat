#ifndef SPLAT_FUNCTIONS_H
#define SPLAT_FUNCTIONS_H

#include "model/Character.h"
#include "model/Platform.h"
#include "model/MovingPlatform.h"
#include "model/GameObjectModel.h"
#include "events/CreateAssetEvent.h"

namespace SPlat {

    namespace Utilities {

        
        static SPlat::Model::Asset& deserialize_asset(std::string serialized) {
            // deserialize args from string
            SPlat::Events::CreateAssetEventArgs args;
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
            return SPlat::Model::GameObjectModel::get_instance().create_asset(*created_asset);
        }

    }

}

#endif