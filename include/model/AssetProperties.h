#ifndef SPLAT_MODEL_ASSETPROPERTIES_H
#define SPLAT_MODEL_ASSETPROPERTIES_H

#include <SFML/Graphics.hpp>

#include <mutex>

namespace SPlat {

    namespace Model {

        class AssetProperties {

        protected:

            std::mutex m;

        public:

            size_t id;

            sf::Vector2f position;

            sf::Vector2f size;

            sf::Color fill_color;

            int collision_priority;


            AssetProperties(AssetProperties& other) {
                set_id(other.get_id());
                set_position(other.get_position());
                set_size(other.get_size());
                set_fill_color(other.get_fill_color());
                set_collision_priority(other.get_collision_priority());
            }

            size_t get_id() {
                m.lock();
                size_t local = id;
                m.unlock();

                return local;
            }

            sf::Vector2f get_position() {
                m.lock();
                sf::Vector2f local = position;
                m.unlock();

                return local;
            }

            sf::Vector2f get_size() {
                m.lock();
                sf::Vector2f local = size;
                m.unlock();

                return local;
            }

            sf::Color get_fill_color() {
                m.lock();
                sf::Color local = fill_color;
                m.unlock();

                return local;
            }

            int get_collision_priority() {
                m.lock();
                int local = collision_priority;
                m.unlock();

                return local;
            }

            void set_id(size_t id) {
                m.lock();
                this->id = id;
                m.unlock();
            }

            void set_position(sf::Vector2f position) {
                m.lock();
                this->position = position;
                m.unlock();
            }

            void set_size(sf::Vector2f size) {
                m.lock();
                this->size = size;
                m.unlock();
            }

            void set_fill_color(sf::Color fill_color) {
                m.lock();
                this->fill_color = fill_color;
                m.unlock();
            }

            void set_collision_priority(int collision_priority) {
                m.lock();
                this->collision_priority = collision_priority;
                m.unlock();
            }

        };

    }

}

#endif