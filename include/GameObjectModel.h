#ifndef SPLAT_GOM
#define SPLAT_GOM

#include <mutex>

#include "Asset.h"

/// NOTE
/// Even though references are passed and stored, all memory management is to 
/// be handled internally by the GOM class. No Asset should ever be created by 
/// a SPlat user in the heap; instead, stack should be passed, which will then 
/// be totally managed by our GOM.

/// I may make the GOM a singleton so that it is easier to deconstruct.

namespace SPlat {

    class GameObjectModel {

        static std::mutex lock;

        static std::unordered_map<size_t, Asset&> assets;

    public:

        static Asset& create_asset(Asset&);

        static Asset& read_asset(Asset&);

        static Asset& update_asset(size_t, Asset&);

        static Asset& delete_asset(size_t);

    };

}

#endif