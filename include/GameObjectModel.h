#ifndef SPLAT_GOM
#define SPLAT_GOM

#include <mutex>

#include "Asset.h"

namespace SPlat {

    class GameObjectModel {

        static std::mutex lock;

        static std::unordered_map<size_t, Asset&> assets;

    public:

        static Asset create_asset(Asset);

        static Asset read_asset(Asset);

        static Asset update_asset(size_t, Asset);

        static Asset delete_asset(size_t);

    };

}

#endif