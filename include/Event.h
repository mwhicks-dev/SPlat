#ifndef SPLAT_EVENT_H
#define SPLAT_EVENT_H

#include <SFML/Graphics.hpp>

namespace SPlat {

    /** Event interface */
    class Event {

    public:

        /** This function is ran whenever an event is processed. */
        virtual void resolve(void) = 0;

        /** Priority of event. Lower integers are higher priority. */
        virtual int get_priority(void) = 0;

        bool operator > (Event &e) {
            return this->get_priority() > e.get_priority();
        }

        bool operator < (Event &e) {
            return this->get_priority() < e.get_priority();
        }

        bool operator >= (Event &e) {
            return this->get_priority() >= e.get_priority();
        }

        bool operator <= (Event &e) {
            return this->get_priority() <= e.get_priority();
        }

        bool operator == (Event &e) {
            return this->get_priority() == e.get_priority();
        }

        bool operator != (Event &e) {
            return this->get_priority() != e.get_priority();
        }

    };

    class SetEvent : public Event {

    public:

        int get_priority(void) override { return 0; }

    };

    class AddEvent : public Event {

    public:

        int get_priority(void) override { return 1; }

    };

    class AssetEvent : public Event {  /// events dealing with assets template

        size_t asset_id;

    public:

        AssetEvent(size_t asset_id) { this->asset_id = asset_id; }

    };

    class VelocityEvent : public AssetEvent {  /// events dealing with velocity

        sf::Vector2f velocity;

    public:

        VelocityEvent(size_t asset_id, sf::Vector2f velocity)
        : AssetEvent(asset_id) { this->velocity = velocity; }

    };

    class PositionEvent : public AssetEvent {  /// events dealing with velocity

        sf::Vector2f position;

    public:

        PositionEvent(size_t asset_id, sf::Vector2f position)
        : AssetEvent(asset_id) { this->position = position; }

    };

    class SetVelocityEvent : public SetEvent, public VelocityEvent {

    public:

        SetVelocityEvent(size_t, sf::Vector2f);

        void resolve() override;

    };

    class AddVelocityEvent : public AddEvent, public VelocityEvent {

    public:

        AddVelocityEvent(size_t, sf::Vector2f);

        void resolve() override;

    };

    class SetPositionEvent : public SetEvent, public PositionEvent {

    public:

        SetPositionEvent(size_t, sf::Vector2f);

        void resolve() override;

    };

    class AddPositionEvent : public AddEvent, public PositionEvent {

    public:

        AddPositionEvent(size_t, sf::Vector2f);

        void resolve() override;

    };

}

#endif