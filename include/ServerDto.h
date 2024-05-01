#ifndef SPlat_ServerDto_h
#define SPlat_ServerDto_h

#include <string>

namespace SPlat {

    struct ServerDto {

        size_t client_size;

        std::string pub_sub_address;

        template <class Archive>
        void serialize(Archive& ar) {
            ar(client_size, pub_sub_address);
        }

    };

}

#endif