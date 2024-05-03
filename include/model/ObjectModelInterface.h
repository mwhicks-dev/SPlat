#ifndef SPlat_ObjectModelInterface_h
#define SPlat_ObjectModelInterface_h

#include "model/Asset.h"

namespace SPlat {

    namespace Model {

        class ObjectModelInterface {

        public:

            virtual Asset& create_asset(Asset&) = 0;

            virtual Asset& read_asset(size_t) = 0;

            virtual Asset& delete_asset(size_t) = 0;

            virtual std::unordered_set<size_t> get_ids() = 0;

        };

    }

}

#endif