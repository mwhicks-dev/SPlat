#include "events/ControlAssetEvent.h"

using namespace SPlat::Events;

std::mutex ControlAssetEvent::controlled_lock;
bool ControlAssetEvent::controlled_set = false;
size_t ControlAssetEvent::controlled = 0;
std::string ControlAssetEvent::TYPE = "control_asset";

void ControlAssetEvent::handler(std::string serialized) {
    // deserialize args from string
    ControlAssetEventArgs args;
    std::stringstream ss; ss << serialized;
    {
        cereal::JSONInputArchive iar(ss);
        iar(args);
    }

    // set model object as controlled
    controlled_lock.lock();
    controlled = args.id;
    controlled_set = true;
    controlled_lock.unlock();
}