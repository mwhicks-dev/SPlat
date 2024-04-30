#ifndef SPLAT_GOM
#define SPLAT_GOM

#include <unordered_map>
#include <unordered_set>
#include <mutex>

#include "model/Asset.h"

namespace SPlat {

    namespace Model {

        class GameObjectModel {

            std::mutex lock;

            std::unordered_set<size_t> ids;

            std::unordered_map<size_t, Asset*> assets;

            GameObjectModel() = default;

            void check_collision(size_t);

        public:

            Asset& create_asset(Asset&);

            Asset& read_asset(size_t);

            Asset& delete_asset(size_t);

            std::unordered_set<size_t> get_ids();

            static GameObjectModel& get_instance();  /// TODO Client composes GOM

        };

    }

}

#endif