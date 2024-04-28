#include "events/Listener.h"

#include "events/AssetEvents.h"
#include "events/CharacterEvents.h"
#include "events/KeyEvents.h"
#include "events/MovingPlatformEvents.h"
#include "events/PlatformEvents.h"
#include "events/TickEvent.h"

#ifdef DEBUG
#include <iostream>
#include <sstream>
#include <cereal/archives/json.hpp>
#endif

using namespace SPlat::Events;

std::mutex Listener::handlers_lock;
std::map<std::string, void (*)(std::string)> Listener::handlers;

Listener::Listener() {
    handlers_lock.lock();
    if (handlers.count(AddPositionEvent::get_type()) == 0)
        handlers[AddPositionEvent::get_type()] = AddPositionEvent::handler;

    if (handlers.count(AddVelocityEvent::get_type()) == 0)
        handlers[AddVelocityEvent::get_type()] = AddVelocityEvent::handler;

    if (handlers.count(ControlAssetEvent::get_type()) == 0)
        handlers[ControlAssetEvent::get_type()] = ControlAssetEvent::handler;

    if (handlers.count(CreateCharacterEvent::get_type()) == 0)
        handlers[CreateCharacterEvent::get_type()] = CreateCharacterEvent::handler;

    if (handlers.count(CreateControlCharacterEvent::get_type()) == 0)
        handlers[CreateControlCharacterEvent::get_type()] = CreateControlCharacterEvent::handler;

    if (handlers.count(CreateMovingPlatformEvent::get_type()) == 0)
        handlers[CreateMovingPlatformEvent::get_type()] = CreateMovingPlatformEvent::handler;

    if (handlers.count(CreatePlatformEvent::get_type()) == 0)
        handlers[CreatePlatformEvent::get_type()] = CreatePlatformEvent::handler;

    if (handlers.count(KeyPressEvent::get_type()) == 0)
        handlers[KeyPressEvent::get_type()] = KeyPressEvent::handler;

    if (handlers.count(KeyReleaseEvent::get_type()) == 0)
        handlers[KeyReleaseEvent::get_type()] = KeyReleaseEvent::handler;

    if (handlers.count(TickEvent::get_type()) == 0)
        handlers[TickEvent::get_type()] = TickEvent::handler;
    handlers_lock.unlock();
}

void Listener::dispatch(Command cmd) {
#ifdef DEBUG
    std::stringstream ss;
    {
        cereal::JSONOutputArchive oar(ss);
        oar(cmd);
    }
    std::cout << "-> Listener::dispatch(" << ss.str() << ")" << std::endl;
#endif
    // get handler string
    bool set = false; 
    void (*handler)(std::string);

    // attempt to retrieve set handler
    handlers_lock.lock();
    set = handlers.count(cmd.type) > 0;
    if (set) handler = handlers[cmd.type];
    handlers_lock.unlock();

    // use set if exists
    if (set) { handler(cmd.args); }
    else throw std::invalid_argument("No such event class " + cmd.type);
#ifdef DEBUG
    std::cout << "<- Listener::dispatch" << std::endl;
#endif
}

void Listener::run() {
    while (true) {
        // break if no events to process
        commands_lock.lock();
        if (commands.empty()) {
            commands_lock.unlock();
            break;
        }
        // dequeue event if nonempty
        Command cmd = commands.top(); commands.pop();
        commands_lock.unlock();

        // dispatch event
        dispatch(cmd);
    }
}

void Listener::push_command(Command cmd) {
#ifdef DEBUG
    std::stringstream ss;
    {
        cereal::JSONOutputArchive oar(ss);
        oar(cmd);
    }
    std::cout << "-> Listener::push_command(" << ss.str() << ")" << std::endl;
#endif
    commands_lock.lock();
    commands.push(cmd);
    commands_lock.unlock();
#ifdef DEBUG
    std::cout << "<- Listener::push_command" << std::endl;
#endif
}

void Listener::set_handler(std::string type, void (*handler)(std::string)) {
#ifdef DEBUG
    std::cout << "-> Listener::set_handler(" << type << ", void (*)(std::string))" << std::endl;
#endif
    handlers_lock.lock();
    handlers[type] = handler;
    handlers_lock.unlock();
#ifdef DEBUG
    std::cout << "<- Listener::set_handler" << std::endl;
#endif
}
