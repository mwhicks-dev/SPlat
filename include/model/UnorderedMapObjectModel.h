#ifndef SPlat_UnorderedMapObjectModel_h
#define SPlat_UnorderedMapObjectModel_h

#include "model/ObjectModelInterface.h"

namespace SPlat {

    namespace Model {

        class UnorderedMapObjectModel : public ObjectModelInterface {

            std::mutex m;

            std::unordered_set<size_t> asset_ids;

            std::unordered_map<size_t, Asset*> game_assets;

            bool causes_collision(size_t);

            void add_id(size_t);

            void remove_id(size_t);

            void add_asset(size_t, Asset*);

            void remove_asset(size_t);

        public:

            Asset& create_asset(Asset&) override;

            Asset& read_asset(size_t) override;

            Asset& delete_asset(size_t) override;

            std::unordered_set<size_t> get_ids() override;

        };

    };

};

#endif