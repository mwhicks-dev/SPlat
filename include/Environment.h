#ifndef SPlat_Environment_h
#define SPlat_Environment_h

#include "EnvironmentInterface.h"

#include <mutex>

namespace SPlat {

    class Environment : public EnvironmentInterface {

        std::mutex m;

        Model::Character * controlled_asset;

        long framerate_limit;

        bool running;

        std::set<sf::Keyboard::Key> held_keys;

        Model::StandingConfigInterface& standing_config;

        float unit;

    public:

        Environment();

        Model::Character * get_controlled_asset() override;

        long get_framerate_limit() override;

        std::set<sf::Keyboard::Key> get_held_keys() override;

        bool get_running() override;

        Model::StandingConfigInterface& get_standing_config() override;

        float get_unit() override;

        void set_controlled_asset(Model::Character *) override;

        void set_framerate_limit(long) override;

        void add_held_key(sf::Keyboard::Key) override;

        void remove_held_key(sf::Keyboard::Key) override;

        void set_running(bool) override;

        void set_unit(float) override;

    };

}

#endif