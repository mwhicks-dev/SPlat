#include "Entrypoint.h"

using namespace SPlat;

std::mutex Entrypoint::m;
Entrypoint * Entrypoint::instance = nullptr;

Entrypoint& Entrypoint::get_instance() {
    const std::lock_guard<std::mutex> lock(m);
    Entrypoint * local = instance;
    
    if (local == nullptr)
        throw std::invalid_argument("No Entrypoint concrete selected");
    
    return *local;
}