#ifndef SPLAT_MODEL_CHARACTERFACTORY_H
#define SPLAT_MODEL_CHARACTERFACTORY_H

#include "model/AbstractMovingFactory.h"
#include "model/UpdateHandler.h"
#include "model/CharacterProperties.h"

namespace SPlat {

    namespace Model {

        class CharacterFactory : public AbstractMovingFactory {

            Asset& create_asset(AssetProperties&) override;

            Asset& update_asset(size_t, AssetProperties&) override;

            class DefaultUpdateHandler : public UpdateHandler {

                void update(time_t) override;

            };

            class DefaultCollisionHandler : public CollisionHandler {

                std::mutex m;

                CharacterProperties * character_properties;

            public:

                DefaultCollisionHandler(CharacterProperties * properties) {
                    set_properties(properties);
                    set_character_properties(properties);
                }

                void resolve_collision(AssetProperties&) override;

                void set_character_properties(CharacterProperties* character_properties) {
                    m.lock();
                    this->character_properties = character_properties;
                    m.unlock();
                }

                CharacterProperties * get_character_properties() {
                    m.lock();
                    CharacterProperties * local = character_properties;
                    m.unlock();

                    return local;
                }

            };

        };

    }

}

#endif