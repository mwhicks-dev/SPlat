#ifndef SPLAT_MODEL_MOVING_H
#define SPLAT_MODEL_MOVING_H

#include "Asset.h"
#include "model/UpdateHandler.h"

namespace SPlat {

    namespace Model {

        class Moving : public Asset {

            UpdateHandler * update_handler = nullptr;

        public:

            Moving(MovingProperties properties, 
                    CollisionHandler& collision_handler,
                    UpdateHandler& update_handler) : Asset(properties, 
                    collision_handler) {
                update_handler.set_properties(&get_moving_properties());
                set_update_handler(update_handler);
            }

            void set_update_handler(UpdateHandler& handler) {
                update_handler = &handler;
            }

            void update(time_t t) {
                update_handler->update(t);
            }

            virtual MovingProperties& get_moving_properties() = 0;

        };

    }

}

#endif