#ifndef SPlat_Events_ClientReadAssetHandler_h
#define SPlat_Events_ClientReadAssetHandler_h

#include "events/handlers/EventHandlerInterface.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class ClientReadAssetHandler : public EventHandlerInterface {

        public:

            struct Args {

                size_t id;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(id);
                }

            };

            void handle(std::string) override;

            static std::string get_type() { return "get_asset"; }

        };

    }

}

#endif
