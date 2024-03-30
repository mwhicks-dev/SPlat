#ifndef SPLAT_GOM
#define SPLAT_GOM

#include <mutex>

#include "Asset.h"

/// NOTE
/// For now, new Assets will be created by the user; later, I may add some top 
/// level interfaces that allow creation of individual GOM types, but will 
/// have to wait and see what is best.

/// I may make the GOM a singleton so that it is easier to deconstruct.

namespace SPlat {

    class GameObjectModel {

        static std::mutex lock;

        static std::unordered_map<size_t, Asset*> assets;

    public:

        static Asset& create_asset(Asset&);

        static Asset& read_asset(size_t);

        static Asset& update_asset(size_t, Asset&);

        static Asset& delete_asset(size_t);

    };

}

#endif