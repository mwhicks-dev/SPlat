#ifndef SPLAT_MODEL_MOVING_H
#define SPLAT_MODEL_MOVING_H

#include "model/Asset.h"
#include "model/MovingProperties.h"
#include "model/handler/UpdateHandler.h"

namespace SPlat {

    namespace Model {

        class Moving : public Asset {

            std::mutex m;

            MovingProperties& properties;

            UpdateHandler * update_handler = nullptr;
        
        protected:

            UpdateHandler * get_update_handler() {
                const std::lock_guard<std::mutex> lock(m);
                return update_handler;
            }

        public:

            Moving(AssetProperties& asset_properties, MovingProperties& moving_properties) 
                    : Asset(asset_properties), properties(moving_properties) {}

            void set_update_handler(UpdateHandler * update_handler) {
                const std::lock_guard<std::mutex> lock(m);
                this->update_handler = update_handler;
            }

            MovingProperties& get_moving_properties() {
                const std::lock_guard<std::mutex> lock(m);
                return properties;
            }

            virtual void update() = 0;

        };

    }

}

#endif