#ifndef SPLAT_SERIALIZATION_H
#define SPLAT_SERIALIZATION_H

#include <SFML/Graphics.hpp>

#include "cereal/cereal.hpp"

#include "events/Event.h"

namespace cereal {

    /// @brief external serialization func for sf::Vector2 
    template<class Archive, typename T>
    void serialize(Archive & ar, sf::Vector2<T> & vec) {
        ar(vec.x, vec.y);
    }

    template <class Archive>
    void serialize(Archive& ar, sf::Color& color) {
        ar(color.r, color.g, color.b, color.a);
    }

}

#endif