#ifndef SPlat_IdDto_h
#define SPlat_IdDto_h

#include <cereal/cereal.hpp>

namespace SPlat {

    struct IdDto {

        size_t id;

        template <class Archive>
        void serialize(Archive& ar) {
            ar(id);
        }

    };

}

#endif