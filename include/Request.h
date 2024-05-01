#ifndef SPlat_Request_h
#define SPlat_Request_h

#include <cereal/types/common.hpp>

namespace SPlat {

    struct Request {

        enum ContentType {

            None,
            Event,
            Connect,
            Disconnect,
            Shutdown,

        };

        ContentType content_type = None;

        std::string body = "";

        template <class Archive>
        void serialize(Archive& ar) {
            ar(content_type, body);
        }

    };

}

#endif