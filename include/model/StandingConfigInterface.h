#ifndef SPlat_Model_StandingConfigInterface_h
#define SPlat_Model_StandingConfigInterface_h

#include <SFML/Graphics.hpp>

namespace SPlat {

    namespace Model {

        class StandingConfigInterface {

        public:

            virtual void push_child(size_t, size_t) = 0;

            virtual void remove_child(size_t, size_t) = 0;

            virtual void push_update_to_children(size_t, sf::Vector2f) = 0;

        };

    }

}

#endif