#ifndef SPLAT_MODEL_CHARACTERFACTORY_H
#define SPLAT_MODEL_CHARACTERFACTORY_H

#include "model/AbstractAssetFactory.h"
#include "model/UpdateHandler.h"

namespace SPlat {

    namespace Model {

        class CharacterFactory : public AbstractAssetFactory {

            Asset& create_asset(AssetProperties&) override;

            Asset& update_asset(size_t, AssetProperties&) override;

            class DefaultUpdateHandler : public UpdateHandler {

                void update(time_t) override;

            };

            class DefaultCollisionHandler : public CollisionHandler {

            public:

                void resolve_collision(AssetProperties&) override;

            };

        };

    }

}

#endif