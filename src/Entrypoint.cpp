#include "Entrypoint.h"
#include <csignal>

using namespace SPlat;

std::mutex Entrypoint::m;
Entrypoint * Entrypoint::instance = nullptr;

void sigint_handler(int f) {
    Entrypoint::get_instance().get_config().get_environment().set_running(false);
}

Entrypoint& Entrypoint::get_instance() {
    const std::lock_guard<std::mutex> lock(m);
    Entrypoint * local = instance;
    
    if (local == nullptr)
        throw std::invalid_argument("No Entrypoint concrete selected");
    signal(SIGINT, sigint_handler);
    
    return *local;
}