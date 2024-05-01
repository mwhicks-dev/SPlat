#ifndef SPlat_Response_h
#define SPlat_Response_h

#include <cereal/types/common.hpp>

namespace SPlat {

    struct Response {

        enum ContentType {

            None,
            Event,
            ServerDto,

        };

        ContentType content_type = ContentType::None;
        
        unsigned short status = 500;

        std::string body = "";

        template <class Archive>
        void serialize(Archive& ar) {
            ar(content_type, status, body);
        }

    };

}

#endif