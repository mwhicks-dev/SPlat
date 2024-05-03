#ifndef SPLAT_MODEL_ASSETPROPERTIES_H
#define SPLAT_MODEL_ASSETPROPERTIES_H

#include "Serialization.h"

#include <mutex>

namespace SPlat {

    namespace Model {

        class AssetProperties {

            std::mutex m;

            size_t id = 0;

            size_t owner = 0;

            sf::Vector2f position = sf::Vector2f(0, 0);

            sf::Vector2f size = sf::Vector2f(50, 50);

            sf::Color fill_color = sf::Color::White;

            int collision_priority = 0;

            time_t updated_time = 0;

            bool loaded = true;

            bool drawable = true;

        public:

            template <class Archive>
            void serialize(Archive& ar) {
                ar(id, owner, position, size, fill_color, collision_priority, updated_time, loaded, drawable);
            }

            AssetProperties() = default;

            AssetProperties(sf::Vector2f position, sf::Vector2f size, 
                    sf::Color fill_color) {
                set_position(position);
                set_size(size);
                set_fill_color(fill_color);
            }

            AssetProperties(AssetProperties& other) {
                set_id(other.get_id());
                set_owner(other.get_owner());
                set_position(other.get_position());
                set_size(other.get_size());
                set_fill_color(other.get_fill_color());
                set_collision_priority(other.get_collision_priority());
                set_loaded(other.get_loaded());
                set_drawable(other.get_drawable());
            }

            size_t get_id() {
                const std::lock_guard<std::mutex> lock(m);
                return id;
            }

            size_t get_owner() {
                const std::lock_guard<std::mutex> lock(m);
                return owner;
            }

            sf::Vector2f get_position() {
                const std::lock_guard<std::mutex> lock(m);
                return position;
            }

            sf::Vector2f get_size() {
                const std::lock_guard<std::mutex> lock(m);
                return size;
            }

            sf::Color get_fill_color() {
                const std::lock_guard<std::mutex> lock(m);
                return fill_color;
            }

            int get_collision_priority() {
                const std::lock_guard<std::mutex> lock(m);
                return collision_priority;
            }

            void set_id(size_t id) {
                const std::lock_guard<std::mutex> lock(m);
                this->id = id;
            }

            void set_owner(size_t owner) {
                const std::lock_guard<std::mutex> lock(m);
                this->owner = owner;
            }

            void set_position(sf::Vector2f position) {
                const std::lock_guard<std::mutex> lock(m);
                this->position = position;
            }

            void set_size(sf::Vector2f size) {
                const std::lock_guard<std::mutex> lock(m);
                this->size = size;
            }

            void set_fill_color(sf::Color fill_color) {
                const std::lock_guard<std::mutex> lock(m);
                this->fill_color = fill_color;
            }

            void set_collision_priority(int collision_priority) {
                const std::lock_guard<std::mutex> lock(m);
                this->collision_priority = collision_priority;
            }

            sf::RectangleShape get_rectangle_shape() {
                sf::RectangleShape r(get_size());
                r.setPosition(get_position());
                r.setFillColor(get_fill_color());
                return r;
            }

            void set_updated_time(time_t updated_time) {
                const std::lock_guard<std::mutex> lock(m);
                this->updated_time = updated_time;
            }

            time_t get_updated_time() {
                const std::lock_guard<std::mutex> lock(m);
                return updated_time;
            }

            void set_loaded(bool loaded) {
                const std::lock_guard<std::mutex> lock(m);
                this->loaded = loaded;
            }

            void set_drawable(bool drawable) {
                const std::lock_guard<std::mutex> lock(m);
                this->drawable = drawable;
            }

            bool get_loaded() {
                const std::lock_guard<std::mutex> lock(m);
                return loaded;
            }

            bool get_drawable() {
                const std::lock_guard<std::mutex> lock(m);
                return drawable;
            }

        };

    }

}

#endif