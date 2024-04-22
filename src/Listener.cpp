#include "events/Listener.h"

#include "events/AssetEvents.h"

using namespace SPlat::Events;

void Listener::dispatch(Command cmd) {
    // get handler string
    bool set = false; 
    void (*handler)(std::string);

    // attempt to retrieve manually set handler
    handlers_lock.lock();
    set = handlers.count(cmd.type) > 0;
    if (set) handler = handlers[cmd.type];
    handlers_lock.unlock();

    // use manually set if exists
    if (set) { handler(cmd.args); return; }

    // otherwise, use defaults
    if (cmd.type == AddPositionEvent::get_type())
        AddPositionEvent::handler(cmd.args);
    if (cmd.type == AddVelocityEvent::get_type())
        AddVelocityEvent::handler(cmd.args);
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
    commands_lock.lock();
    commands.push(cmd);
    commands_lock.unlock();
}

void Listener::set_handler(std::string type, void (*handler)(std::string)) {
    handlers_lock.lock();
    handlers[type] = handler;
    handlers_lock.unlock();
}
