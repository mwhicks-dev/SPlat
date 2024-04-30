#include "Entrypoint.h"

using namespace SPlat;

std::mutex Entrypoint::m;
Entrypoint * Entrypoint::instance = nullptr;

Entrypoint& Entrypoint::get_instance() {
    m.lock();
    Entrypoint * local = instance;
    m.unlock();

    if (local == nullptr)
        throw std::invalid_argument("No Entrypoint concrete selected");
    
    return *local;
}