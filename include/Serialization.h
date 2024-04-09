#include "cereal/cereal.hpp"

#include <SFML/Graphics.hpp>

namespace cereal {

    /// @brief external serialization func for sf::Vector2 
    template<class Archive, typename T>
    void serialize(Archive & ar, sf::Vector2<T> & vec) {
        ar(vec.x, vec.y);
    }

}

