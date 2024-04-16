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

    /// @brief external serialization func for SPlat::Events::Event
    template <class Archive>
    void serialize(Archive& ar, SPlat::Events::Event & ev) {
        ar(ev.type, ev.args, ev.foreground);
    }

}

#endif