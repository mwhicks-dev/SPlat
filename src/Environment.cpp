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
    const std::lock_guard<std::mutex> lock(m);
    Model::Character * local = controlled_asset;
    
    return local;
}

std::set<sf::Keyboard::Key> Environment::get_held_keys() {
    const std::lock_guard<std::mutex> lock(m);
    return held_keys;
}

bool Environment::get_running() {
    const std::lock_guard<std::mutex> lock(m);
    return running;
}

Model::StandingConfigInterface& Environment::get_standing_config() {
    const std::lock_guard<std::mutex> lock(m);
    return standing_config;
}

float Environment::get_unit() {
    const std::lock_guard<std::mutex> lock(m);
    return unit;
}

void Environment::set_controlled_asset(Model::Character * controlled_asset) {
#ifdef DEBUG
    std::cout << "-> Environment::set_controlled_asset(Model::Character*)" << std::endl;
#endif
    const std::lock_guard<std::mutex> lock(m);
    this->controlled_asset = controlled_asset;
#ifdef DEBUG
    std::cout << "<- Environment::set_controlled_asset" << std::endl;
#endif
}

void Environment::add_held_key(sf::Keyboard::Key key) {
#ifdef DEBUG
    std::cout << "-> Environment::add_held_key(sf::Keyboard::Key)" << std::endl;
#endif
    const std::lock_guard<std::mutex> lock(m);
    held_keys.insert(key);
#ifdef DEBUG
    std::cout << "<- Environment::add_held_key" << std::endl;
#endif
}

void Environment::remove_held_key(sf::Keyboard::Key key) {
#ifdef DEBUG
    std::cout << "-> Environment::remove_held_key(sf::Keyboard::Key)" << std::endl;
#endif
    const std::lock_guard<std::mutex> lock(m);
    held_keys.erase(key);
#ifdef DEBUG
    std::cout << "<- Environment::remove_held_key" << std::endl;
#endif
}

void Environment::set_running(bool running) {
#ifdef DEBUG
    std::cout << "-> set_running(" << running << ")" << std::endl;
#endif
    const std::lock_guard<std::mutex> lock(m);
    this->running = running;
#ifdef DEBUG
    std::cout << "<- set_running" << std::endl;
#endif
}

void Environment::set_unit(float unit) {
#ifdef DEBUG
    std::cout << "-> Environment::set_unit(" << unit << std::endl;
#endif
    const std::lock_guard<std::mutex> lock(m);
    this->unit = unit;
#ifdef DEBUG
    std::cout << "<- Environment::set_unit" << std::endl;
#endif
}

size_t Environment::get_entrypoint_id() {
    const std::lock_guard<std::mutex> lock(m);
    return entrypoint_id;
}

std::string Environment::get_pub_sub_addres() {
    const std::lock_guard<std::mutex> lock(m);
    return pub_sub_address;
}

std::string Environment::get_req_rep_addres() {
    const std::lock_guard<std::mutex> lock(m);
    return req_rep_address;
}

void Environment::set_entrypoint_id(size_t entrypoint_id) {
#ifdef DEBUG
    std::cout << "-> Environment::set_entrypoint_id(" << entrypoint_id << ")" << std::endl;
#endif
    const std::lock_guard<std::mutex> lock(m);
    this->entrypoint_id = entrypoint_id;
#ifdef DEBUG
    std::cout << "<- Environment::set_entrypoint_id" << std::endl;
#endif
}

void Environment::set_pub_sub_addres(std::string pub_sub_address) {
#ifdef DEBUG
    std::cout << "-> Environment::set_pub_sub_addres(" << pub_sub_address << ")" << std::endl;
#endif
    const std::lock_guard<std::mutex> lock(m);
    this->pub_sub_address = pub_sub_address;
#ifdef DEBUG
    std::cout << "<- Environment::set_pub_sub_addres" << std::endl;
#endif
}

void Environment::set_req_rep_address(std::string req_rep_address) {
#ifdef DEBUG
    std::cout << "-> Environment::set_req_rep_address(" << req_rep_address << ")" << std::endl;
#endif
    const std::lock_guard<std::mutex> lock(m);
    this->req_rep_address = req_rep_address;
#ifdef DEBUG
    std::cout << "<- Environment::set_req_rep_address" << std::endl;
#endif
}

std::map<size_t, std::string> Environment::get_contexts() {
    const std::lock_guard<std::mutex> lock(m);
    return contexts;
}

void Environment::set_contexts(std::map<size_t, std::string> contexts) {
    const std::lock_guard<std::mutex> lock(m);
    this->contexts = contexts;
}

std::string Environment::get_context(size_t key) {
    auto local = get_contexts();
    if (local.count(key) == 0) return "";

    return local[key];
}

void Environment::set_context(size_t key, std::string value) {
    auto local = get_contexts();
    local[key] = value;
    set_contexts(local);
}

void Environment::remove_context(size_t key) {
    auto local = get_contexts();
    local.erase(key);
    set_contexts(local);
}
