#ifndef SPlat_DisconnectDto_h
#define SPlat_DisconnectDto_h

#include <cereal/cereal.hpp>

namespace SPlat {

    struct DisconnectDto {

        size_t entrypoint_id;

        template <class Archive>
        void serialize(Archive& ar) {
            ar(entrypoint_id);
        }

    };

}

#endif