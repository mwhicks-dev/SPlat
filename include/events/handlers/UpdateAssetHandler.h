#ifndef UpdateAssetHandler_h
#define UpdateAssetHandler_h

#include "events/handlers/EventHandlerInterface.h"
#include "model/AssetProperties.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class UpdateAssetHandler : public EventHandlerInterface {

        public:

            struct Args {

                size_t id;

                SPlat::Model::AssetProperties properties;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(id, properties);
                }

            };

            void handle(std::string) = 0;

            static std::string get_type() { return "update_asset"; }

        };

    }

}

#endif
