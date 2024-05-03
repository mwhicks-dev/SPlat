#ifndef SPlat_DespawnAssetHandler_h
#define SPlat_DespawnAssetHandler_h

#include "EventHandlerInterface.h"

#include <cereal/cereal.hpp>

namespace SPlat {

    namespace Events {

        class DespawnAssetHandler : public EventHandlerInterface {

        public:

            struct Args {

                size_t asset_id;

                template <class Archive>
                void serialize(Archive& ar) {
                    ar(asset_id);
                }

            };

            void handle(std::string) = 0;

            static std::string get_type() { return "despawn_asset"; }

        };

    }

}

#endif