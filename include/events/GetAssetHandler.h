#ifndef SPlat_Events_GetAssetHandler_h
#define SPlat_Events_GetAssetHandler_h

#include "events/CommandHandlerInterface.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class GetAssetHandler : public CommandHandlerInterface {

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
