#include "events/OrderedPriorityListener.h"
#include "Entrypoint.h"

#include <cereal/archives/json.hpp>

#include <sstream>
#include <thread>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Events;

SPlat::Event OrderedPriorityListener::poll_event() {
    const std::lock_guard<std::mutex> lock(n);
    auto local = event_queue.top();
    event_queue.pop();
    
    return local;
}

EventHandlerInterface* OrderedPriorityListener::get_handler(std::string type) {
    const std::lock_guard<std::mutex> lock(m);
    if (handlers.count(type) == 0)
        throw std::invalid_argument("Listener has no set handler for type " + type);
    
    return handlers[type];
}

bool OrderedPriorityListener::command_available() {
    const std::lock_guard<std::mutex> lock(n);
    return !event_queue.empty();
}

void OrderedPriorityListener::listener_loop() {
#ifdef DEBUG
    std::cout << "-> OrderedPriorityListener::listener_loop()" << std::endl;
#endif
    EnvironmentInterface& env = Entrypoint::get_instance().get_config().get_environment();

    while (env.get_running()) {
        if (command_available()) {
            Event curr = poll_event();
            std::stringstream ss;
            {
                cereal::JSONOutputArchive oar(ss);
                oar(curr);
            }
            try {
                await(curr);
            } catch (std::exception& e) {
                std::cout << "Warning: Listener was unable to process the following event: " << std::endl;
                std::cout << "  type: " << curr.command.type << std::endl;
                std::cout << "  args: " << std::endl;
                std::cout << "```" << std::endl;
                std::cout << curr.command.args << std::endl;
                std::cout << "```" << std::endl;
                std::cout << "raised exception " << e.what() << std::endl;
            }
        }
    }
#ifdef DEBUG
    std::cout << "<- OrderedPriorityListener::listener_loop" << std::endl;
#endif
}

OrderedPriorityListener::OrderedPriorityListener()
: handlers(*new std::map<std::string, EventHandlerInterface*>) {}

void OrderedPriorityListener::set_handler(std::string type, EventHandlerInterface& handler) {
#ifdef DEBUG
    std::cout << "-> OrderedPriorityListener::set_handler(" << type << ", EventHandlerInterface&)" << std::endl;
#endif
    const std::lock_guard<std::mutex> lock(m);
    handlers[type] = &handler;
#ifdef DEBUG
    std::cout << "<- OrderedPriorityListener::set_handler" << std::endl;
#endif
}

void OrderedPriorityListener::push_event(Event event) {
#ifdef DEBUG
    std::cout << "-> OrderedPriorityListener::push_command(Command)" << std::endl;
#endif
    const std::lock_guard<std::mutex> lock(n);
    event_queue.push(event);
#ifdef DEBUG
    std::cout << "<- OrderedPriorityListener::push_command(Command)" << std::endl;
#endif
}

void OrderedPriorityListener::run() {
#ifdef DEBUG
    std::cout << "-> OrderedPriorityListener::run()" << std::endl;
#endif
    std::thread t(&OrderedPriorityListener::listener_loop, this);
    t.detach();
#ifdef DEBUG
    std::cout << "<- OrderedPriorityListener::run" << std::endl;
#endif
}

void OrderedPriorityListener::await(Event event) {
#ifdef DEBUG
    std::cout << "-> OrderedPriorityListener::await(Command)" << std::endl;
#endif
    EventHandlerInterface * handler = get_handler(event.command.type);
    std::stringstream event_ss;
    {
        cereal::JSONOutputArchive oar(event_ss);
        oar(event);
    }
    {
        const std::lock_guard<std::mutex> lock(m);
        handler->handle(event_ss.str());
    }
#ifdef DEBUG
    std::cout << "<- OrderedPriorityListener::await" << std::endl;
#endif
}
