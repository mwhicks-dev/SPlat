#include "model/MovingPlatform.h"
#include "events/AssetEvents.h"
#include <cmath>

#ifdef DEBUG
#include <iostream>
#endif

using namespace SPlat::Model;

MovingPlatform::MovingPlatform(sf::Vector2f& size)
: Platform(size) {}

int MovingPlatform::get_priority() { return -1; }

std::string MovingPlatform::TYPE = "moving_platform";

std::string MovingPlatform::get_type() { return MovingPlatform::TYPE; }

void MovingPlatform::update() {
#ifdef DEBUG
    std::cout << "-. MovingPlatform::update()" << std::endl;
#endif
    // if queue length is zero, do nothing
    if (queue.size() == 0) return;

    // get current state
    State curr = queue[0];

    // get difference of next and curr positions
    size_t remaining_ticks = curr.ticks_til_next - platform_ticks;

    // if tick limit reached, pop first and restart
    if (remaining_ticks == 0) {
        queue.erase(queue.begin());
        if (curr.repeat) queue.push_back(curr);
        platform_ticks = 0;
        return update();
    }

    // get next state
    State next;
    if (queue.size() == 1) {  // only one queue element
        // do nothing if curr is not repeatable
        if (!curr.repeat) return;
        // o/w next is curr
        next = curr;
    } else {  // multiple elements
        next = queue[1];
    }

    // divide difference between next and curr by number of remaining ticks
    sf::Vector2f move = (next.position - getPosition()) 
        / static_cast<float>(remaining_ticks);
    
    // move platform by vector
    SPlat::Events::AddPositionEvent event(id, move);
    event.raise();
    for (size_t stander_id : standers) {
        event = SPlat::Events::AddPositionEvent(stander_id, move);
        event.raise();
    }
    platform_ticks += 1;
#ifdef DEBUG
    std::cout << "<- MovingPlatform::update" << std::endl;
#endif
}
