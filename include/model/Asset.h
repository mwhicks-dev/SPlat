#ifndef SPLAT_ASSET
#define SPLAT_ASSET

#include <SFML/Graphics.hpp>

#include "Serialization.h"

namespace SPlat {

    namespace Model {

        /// @brief communicable properties to use for Asset events
        struct AssetProperties {

            /// @brief string type of asset
            std::string type;

            /// @brief asset size vector
            sf::Vector2f size;

            /// @brief asset position vector
            sf::Vector2f position;

            /// @brief convert AssetProperties to serializable string
            /// @tparam Archive cereal archive class
            /// @param ar Archive to use for AssetProperties conversion
            template <class Archive>
            void serialize(Archive& ar) {
                ar(type, size, position);
            }
        };

        class Asset : public sf::RectangleShape {

        public:

            size_t id;

            Asset * standing_on;

            sf::Vector2f velocity;

            Asset(sf::Vector2f&);

            virtual int get_priority() = 0;

            virtual std::string get_type() = 0;

            AssetProperties get_properties() {
                return {get_type(), getSize(), getPosition()};
            }

        };

    }

}

#endif