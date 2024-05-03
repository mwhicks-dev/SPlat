#ifndef SPlat_Events_ReadAssetHandler_h
#define SPlat_Events_ReadAssetHandler_h

#include "events/handlers/EventHandlerInterface.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class ReadAssetHandler : public EventHandlerInterface {

        public:

            struct Args {

                size_t id;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(id);
                }

            };

            virtual void handle(std::string) = 0;

            static std::string get_type() { return "get_asset"; }

        };

    }

}

#endif
