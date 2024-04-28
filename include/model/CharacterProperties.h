#ifndef SPLAT_MODEL_CHARACTERPROPERTIES_H
#define SPLAT_MODEL_CHARACTERPROPERTIES_H

#include "MovingProperties.h"

namespace SPlat {

    namespace Model {

        class CharacterProperties : public MovingProperties {

        public:

            AssetProperties * standing_on = nullptr;

            CharacterProperties(CharacterProperties& other) 
            : MovingProperties(other) {
                set_standing_on(other.get_standing_on());
            }

            AssetProperties * get_standing_on() {
                m.lock();
                AssetProperties * local = standing_on;
                m.unlock();

                return local;
            }

            void set_standing_on(AssetProperties * standing_on) {
                m.lock();
                this->standing_on = standing_on;
                m.unlock();
            }

        };

    }

}

#endif