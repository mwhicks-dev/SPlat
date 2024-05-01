#include "events/OrderedPriorityListener.h"
#include "Entrypoint.h"

#include <thread>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Events;

Command OrderedPriorityListener::poll_command() {
    m.lock();
    auto local = command_queue.top();
    command_queue.pop();
    m.unlock();

    return local;
}

CommandHandlerInterface* OrderedPriorityListener::get_handler(std::string type) {
    CommandHandlerInterface * local = nullptr;
    m.lock();
    if (handlers.count(type) > 0)
        local = handlers[type];
    m.unlock();

    if (local == nullptr)
        throw std::invalid_argument("Listener has no set handler for type " + type);

    return local;
}

bool OrderedPriorityListener::command_available() {
    m.lock();
    auto local = !command_queue.empty();
    m.unlock();

    return local;
}

void OrderedPriorityListener::listener_loop() {
#ifdef DEBUG
    std::cout << "-> OrderedPriorityListener::listener_loop()" << std::endl;
#endif
    EnvironmentInterface& env = Entrypoint::get_instance().get_config().get_environment();

    while (env.get_running()) {
        if (command_available()) {
            Command curr = poll_command();
            try {
                CommandHandlerInterface* handler = get_handler(curr.type);
                handler->handle(curr.args);
            } catch (std::exception& e) {
                std::cout << "Warning: Listener was unable to process the following event: " << std::endl;
                std::cout << "  type: " << curr.type << std::endl;
                std::cout << "  args: " << std::endl;
                std::cout << "```" << std::endl;
                std::cout << curr.args << std::endl;
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
: handlers(*new std::map<std::string, CommandHandlerInterface*>) {}

void OrderedPriorityListener::set_handler(std::string type, CommandHandlerInterface& handler) {
#ifdef DEBUG
    std::cout << "-> OrderedPriorityListener::set_handler(" << type << ", CommandHandlerInterface&)" << std::endl;
#endif
    m.lock();
    handlers[type] = &handler;
    m.unlock();
#ifdef DEBUG
    std::cout << "<- OrderedPriorityListener::set_handler" << std::endl;
#endif
}

void OrderedPriorityListener::push_command(Command cmd) {
#ifdef DEBUG
    std::cout << "-> OrderedPriorityListener::push_command(Command)" << std::endl;
#endif
    m.lock();
    command_queue.push(cmd);
    m.unlock();
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

void OrderedPriorityListener::await(Command cmd) {
#ifdef DEBUG
    std::cout << "-> OrderedPriorityListener::await(Command)" << std::endl;
#endif
    CommandHandlerInterface * handler = get_handler(cmd.type);
    handler->handle(cmd.args);
#ifdef DEBUG
    std::cout << "<- OrderedPriorityListener::await" << std::endl;
#endif
}
