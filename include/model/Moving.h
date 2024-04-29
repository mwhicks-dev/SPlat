#ifndef SPLAT_MODEL_MOVING_H
#define SPLAT_MODEL_MOVING_H

#include "Asset.h"
#include "model/MovingProperties.h"

namespace SPlat {

    namespace Model {

        class Moving : public Asset {

            std::mutex m;

            MovingProperties& properties;

        public:

            Moving(AssetProperties& asset_properties, MovingProperties& moving_properties) 
                    : Asset(asset_properties), properties(moving_properties) {}

            MovingProperties& get_moving_properties() {
                m.lock();
                MovingProperties& local = properties;
                m.unlock();

                return local;
            }

            virtual void update() = 0;

        };

    }

}

#endif