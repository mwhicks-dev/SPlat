#ifndef SPlat_Environment_h
#define SPlat_Environment_h

#include "EnvironmentInterface.h"

#include <mutex>

namespace SPlat {

    class Environment : public EnvironmentInterface {

        std::mutex m;

        Model::Character * controlled_asset;

        bool running;

        std::set<sf::Keyboard::Key> held_keys;

        Model::StandingConfigInterface& standing_config;

        float unit;

        size_t entrypoint_id;

        std::string pub_sub_address = "";

        std::string req_rep_address = "";

    public:

        Environment();

        Model::Character * get_controlled_asset() override;

        std::set<sf::Keyboard::Key> get_held_keys() override;

        bool get_running() override;

        Model::StandingConfigInterface& get_standing_config() override;

        float get_unit() override;

        void set_controlled_asset(Model::Character *) override;

        void add_held_key(sf::Keyboard::Key) override;

        void remove_held_key(sf::Keyboard::Key) override;

        void set_running(bool) override;

        void set_unit(float) override;

        size_t get_entrypoint_id() override;

        std::string get_pub_sub_addres() override;

        std::string get_req_rep_addres() override;

        void set_entrypoint_id(size_t) override;

        void set_pub_sub_addres(std::string) override;

        void set_req_rep_address(std::string) override;

    };

}

#endif