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
                m.lock();
                UpdateHandler * local = update_handler;
                m.unlock();

                return local;
            }

        public:

            Moving(AssetProperties& asset_properties, MovingProperties& moving_properties) 
                    : Asset(asset_properties), properties(moving_properties) {}

            void set_update_handler(UpdateHandler * update_handler) {
                m.lock();
                this->update_handler = update_handler;
                m.unlock();
            }

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