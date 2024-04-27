#include <chrono>

namespace SPlat {

    /// @brief base timeline interface
    class Timeline {

    public:

        /// @brief interface method for getting time
        /// @return time of implementation timeline
        virtual time_t get_time() = 0;

    };

}