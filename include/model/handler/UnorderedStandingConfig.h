#ifndef SPlat_Model_UnorderedStandingConfig_h
#define SPlat_Model_UnorderedStandingConfig_h

#include "model/handler/StandingConfigInterface.h"

#include <unordered_map>
#include <unordered_set>
#include <mutex>

namespace SPlat {

    namespace Model {

        class UnorderedStandingConfig : public StandingConfigInterface {

            std::mutex m;

            std::unordered_map<size_t, std::unordered_set<size_t>> coupled;

            std::unordered_map<size_t, std::unordered_set<size_t>> get_coupled();

            void set_coupled(std::unordered_map<size_t, std::unordered_set<size_t>>);

        public:

            void push_child(size_t, size_t) override;

            void remove_child(size_t, size_t) override;

            void push_update_to_children(size_t, sf::Vector2f) override;

        };

    }

}

#endif