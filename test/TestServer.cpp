#include "events/handlers/ServerCreatePlatformHandler.h"
#include "events/handlers/ServerCreateMovingPlatformHandler.h"
#include "Server.h"

#include <cereal/archives/json.hpp>

using namespace SPlat;

int main() {
    Server& server = server.get_instance();
    ConfigInterface& conf = server.get_config();

    {
        Model::AssetProperties properties(
            sf::Vector2f(0, 500),  // position
            sf::Vector2f(400, 100),  // size
            sf::Color::Green  // fill_color
        );
        Events::ServerCreatePlatformHandler::Args args = {
            .properties=properties
        };
        std::stringstream ss;
        {
            cereal::JSONOutputArchive oar(ss);
            oar(args);
        }
        Events::Command cmd = {
            .priority=-1,
            .type=Events::ServerCreatePlatformHandler::get_type(),
            .args=ss.str()
        };
        Event event = {
            .command=cmd,
            .sender=1,
        };
        server.get_background_listener().push_event(event);
    }
    {
        Model::AssetProperties properties(
            sf::Vector2f(200, 300),  // position
            sf::Vector2f(150, 25),  // size
            sf::Color::White  // fill_color
        );
        Events::ServerCreateMovingPlatformHandler::Args args = {
            .properties=properties
        };

        // add states
        std::vector<Model::State> states;
        states.push_back(
            Model::State(
                sf::Vector2f(5, 0) * conf.get_environment().get_unit(),  // velocity
                1.5 * conf.get_timing_config().get_anchor_steps_per_second(),  // anchor_steps
                true  // repeat
            )
        );
        states.push_back(
            Model::State(
                sf::Vector2f(-15, 0) * conf.get_environment().get_unit(),  // velocity
                0.5 * conf.get_timing_config().get_anchor_steps_per_second(),  // anchor_steps
                true  // repeat
            )
        );
        args.states = states;
        std::stringstream ss;
        {
            cereal::JSONOutputArchive oar(ss);
            oar(args);
        }
        Events::Command cmd = {
            .priority=-1,
            .type=Events::ServerCreateMovingPlatformHandler::get_type(),
            .args=ss.str()
        };
        Event event = {
            .command=cmd,
            .sender=1,
        };
        server.get_background_listener().push_event(event);
    }

    server.start();
}