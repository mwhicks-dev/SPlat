#ifndef SPlat_ServerDto_h
#define SPlat_ServerDto_h

#include <string>

namespace SPlat {

    struct ServerDto {

        size_t client_id = 0;

        std::string pub_sub_address = "";

        template <class Archive>
        void serialize(Archive& ar) {
            ar(client_id, pub_sub_address);
        }

    };

}

#endif