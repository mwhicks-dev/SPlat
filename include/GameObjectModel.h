#ifndef SPLAT_GOM
#define SPLAT_GOM

#include <unordered_map>
#include <mutex>

#include "Asset.h"

/// NOTE
/// For now, new Assets will be created by the user; later, I may add some top 
/// level interfaces that allow creation of individual GOM types, but will 
/// have to wait and see what is best.

/// I may make the GOM a singleton so that it is easier to deconstruct.

namespace SPlat {

    class GameObjectModel {

        std::mutex lock;

        std::unordered_map<size_t, Asset*> assets;

        GameObjectModel() = default;

    public:

        Asset& create_asset(Asset&);

        Asset& read_asset(size_t);

        Asset& update_asset(size_t, Asset&);

        Asset& delete_asset(size_t);

        /// @ref: https://stackoverflow.com/questions/1008019/how-do-you-implement-the-singleton-design-pattern/1008289#1008289
        static GameObjectModel& get_instance() {
            static GameObjectModel instance;
            return instance;
        }

        ~GameObjectModel() {
            for (std::pair<size_t, Asset*> entry : assets) {
                delete entry.second;
            }
        }

    };

}

#endif