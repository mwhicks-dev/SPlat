#include "Environment.h"
#include "model/handler/UnorderedStandingConfig.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat;

Environment::Environment()
        : standing_config(*new Model::UnorderedStandingConfig()) {
    set_controlled_asset(nullptr);
    set_running(true);
    set_unit(50);
}

Model::Character * Environment::get_controlled_asset() {
    m.lock();
    Model::Character * local = controlled_asset;
    m.unlock();

    return local;
}

std::set<sf::Keyboard::Key> Environment::get_held_keys() {
    m.lock();
    auto local = held_keys;
    m.unlock();

    return local;
}

bool Environment::get_running() {
    m.lock();
    bool local = running;
    m.unlock();

    return local;
}

Model::StandingConfigInterface& Environment::get_standing_config() {
    m.lock();
    auto& local = standing_config;
    m.unlock();

    return local;
}

float Environment::get_unit() {
    m.lock();
    float local = unit;
    m.unlock();

    return local;
}

void Environment::set_controlled_asset(Model::Character * controlled_asset) {
#ifdef DEBUG
    std::cout << "-> Environment::set_controlled_asset(Model::Character*)" << std::endl;
#endif
    m.lock();
    this->controlled_asset = controlled_asset;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- Environment::set_controlled_asset" << std::endl;
#endif
}

void Environment::add_held_key(sf::Keyboard::Key key) {
#ifdef DEBUG
    std::cout << "-> Environment::add_held_key(sf::Keyboard::Key)" << std::endl;
#endif
    m.lock();
    held_keys.insert(key);
    m.unlock();
#ifdef DEBUG
    std::cout << "<- Environment::add_held_key" << std::endl;
#endif
}

void Environment::remove_held_key(sf::Keyboard::Key key) {
#ifdef DEBUG
    std::cout << "-> Environment::remove_held_key(sf::Keyboard::Key)" << std::endl;
#endif
    m.lock();
    held_keys.erase(key);
    m.unlock();
#ifdef DEBUG
    std::cout << "<- Environment::remove_held_key" << std::endl;
#endif
}

void Environment::set_running(bool running) {
#ifdef DEBUG
    std::cout << "-> set_running(" << running << ")" << std::endl;
#endif
    m.lock();
    this->running = running;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- set_running" << std::endl;
#endif
}

void Environment::set_unit(float unit) {
#ifdef DEBUG
    std::cout << "-> Environment::set_unit(" << unit << std::endl;
#endif
    m.lock();
    this->unit = unit;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- Environment::set_unit" << std::endl;
#endif
}

size_t Environment::get_entrypoint_id() {
    m.lock();
    auto local = entrypoint_id;
    m.unlock();

    return local;
}

std::string Environment::get_pub_sub_addres() {
    m.lock();
    auto local = pub_sub_address;
    m.unlock();

    return local;
}

std::string Environment::get_req_rep_addres() {
    m.lock();
    auto local = req_rep_address;
    m.unlock();

    return local;
}

void Environment::set_entrypoint_id(size_t entrypoint_id) {
#ifdef DEBUG
    std::cout << "-> Environment::set_entrypoint_id(" << entrypoint_id << ")" << std::endl;
#endif
    m.lock();
    this->entrypoint_id = entrypoint_id;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- Environment::set_entrypoint_id" << std::endl;
#endif
}

void Environment::set_pub_sub_addres(std::string pub_sub_address) {
#ifdef DEBUG
    std::cout << "-> Environment::set_pub_sub_addres(" << pub_sub_address << ")" << std::endl;
#endif
    m.lock();
    this->pub_sub_address = pub_sub_address;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- Environment::set_pub_sub_addres" << std::endl;
#endif
}

void Environment::set_req_rep_address(std::string req_rep_address) {
#ifdef DEBUG
    std::cout << "-> Environment::set_req_rep_address(" << req_rep_address << ")" << std::endl;
#endif
    m.lock();
    this->req_rep_address = req_rep_address;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- Environment::set_req_rep_address" << std::endl;
#endif
}
