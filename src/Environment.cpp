#include "Environment.h"
#include "model/handler/UnorderedStandingConfig.h"

using namespace SPlat;

Environment::Environment()
        : standing_config(*new Model::UnorderedStandingConfig()) {
    set_controlled_asset(nullptr);
    set_running(true);
    set_framerate_limit(60);
    set_unit(50);
}

Model::Character * Environment::get_controlled_asset() {
    m.lock();
    Model::Character * local = controlled_asset;
    m.unlock();

    return local;
}

long Environment::get_framerate_limit() {
    m.lock();
    long local = framerate_limit;
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
    m.lock();
    this->controlled_asset = controlled_asset;
    m.unlock();
}

void Environment::set_framerate_limit(long framerate_limit) {
    m.lock();
    this->framerate_limit = framerate_limit;
    m.unlock();
}

void Environment::set_running(bool running) {
    m.lock();
    this->running = running;
    m.unlock();
}

void Environment::set_unit(float unit) {
    m.lock();
    this->unit = unit;
    m.unlock();
}
