#ifndef SPLAT_MODEL_CHARACTERPROPERTIES_H
#define SPLAT_MODEL_CHARACTERPROPERTIES_H

#include "model/AssetProperties.h"

namespace SPlat {

    namespace Model {

        class CharacterProperties {

            std::mutex m;

        public:

            AssetProperties * standing_on = nullptr;

            CharacterProperties() = default;

            CharacterProperties(AssetProperties* standing_on) {
                set_standing_on(standing_on);
            }

            CharacterProperties(CharacterProperties& other)  {
                set_standing_on(other.get_standing_on());
            }

            AssetProperties * get_standing_on() {
                const std::lock_guard<std::mutex> lock(m);
                return standing_on;
            }

            void set_standing_on(AssetProperties * standing_on) {
                const std::lock_guard<std::mutex> lock(m);
                this->standing_on = standing_on;
            }

        };

    }

}

#endif