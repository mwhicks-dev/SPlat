#ifndef SPlat_Events_UpdateAssetHandler_h
#define SPlat_Events_UpdateAssetHandler_h

#include "events/EventHandlerInterface.h"
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

            void handle(std::string) override;

            static std::string get_type() { return "update_asset"; }

        };

    }

}

#endif
