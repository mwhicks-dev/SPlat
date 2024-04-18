#ifndef SPLAT_MODEL_ASSETFACTORY_H
#define SPLAT_MODEL_ASSETFACTORY_H

#include "model/GameObjectModel.h"

namespace SPlat {

    namespace Model {

        /// @brief factory class for concrete asset CRUD
        /// @tparam T class to ship to GOM
        template <class T>
        class AssetFactory {

        public:

            /// @brief create asset by template
            /// @param tmp asset template to create from
            /// @return asset by value
            static T create_asset(T tmp) {
                T* heap = new T(tmp);
                heap = (T*) &GameObjectModel::get_instance().create_asset(*heap);
                return *heap;
            }

            /// @brief read asset by ID
            /// @param id asset ID to read
            /// @return asset by value
            static T read_asset(size_t id) {
                return (T&) GameObjectModel::get_instance().read_asset(id);
            }

            /// @brief update asset by ID and by template
            /// @param id asset ID to update
            /// @param tmp asset template to update from
            /// @return asset by value
            static T update_asset(size_t id, T tmp) {
                return (T&) GameObjectModel::get_instance()
                    .update_asset(id, tmp);
            }

            /// @brief delete asset by ID
            /// @param id asset ID to delete
            /// @return asset by value
            static T delete_asset(size_t id) {
                return (T&) GameObjectModel::get_instance().delete_asset(id);
            }

        };

    }

}

#endif