#ifndef SPlat_EnvironmentInterface_h
#define SPlat_EnvironmentInterface_h

#include "model/Character.h"
#include "model/handler/StandingConfigInterface.h"

#include <SFML/Window/Keyboard.hpp>
#include <set>

namespace SPlat {

    class EnvironmentInterface {

    public:

        virtual Model::Character * get_controlled_asset() = 0;

        virtual size_t get_entrypoint_id() = 0;

        virtual std::set<sf::Keyboard::Key> get_held_keys() = 0;

        virtual std::string get_pub_sub_addres() = 0;

        virtual std::string get_req_rep_addres() = 0;

        virtual bool get_running() = 0;

        virtual Model::StandingConfigInterface& get_standing_config() = 0;

        virtual float get_unit() = 0;

        virtual void set_controlled_asset(Model::Character *) = 0;

        virtual void set_entrypoint_id(size_t) = 0;

        virtual void add_held_key(sf::Keyboard::Key) = 0;

        virtual void remove_held_key(sf::Keyboard::Key) = 0;

        virtual void set_pub_sub_addres(std::string) = 0;

        virtual void set_req_rep_address(std::string) = 0;

        virtual void set_running(bool) = 0;

        virtual void set_unit(float) = 0;

        virtual std::string get_context(size_t) = 0;

        virtual void set_context(size_t, std::string) = 0;

        virtual void remove_context(size_t) = 0;

    };

}

#endif