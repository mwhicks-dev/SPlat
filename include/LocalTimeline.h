#include "Timeline.h"

#include <mutex>

namespace SPlat {

    /// @brief timeline used for loops, display, and so on
    class LocalTimeline {

        /// @brief safeguards for potential shared access
        std::mutex m;

        /// @brief anchor timeline passed in constructor
        Timeline& anchor;

        /// @brief anchor time on initialize
        time_t start_time;

        /// @brief last time paused was set to true
        time_t last_paused_time;

        /// @brief long number of anchor dt per this dt 
        time_t tic;

        /// @brief whether or not currently paused
        bool paused;

        /// @brief setter for paused field
        /// @param paused new value of paused
        void set_paused(bool);

        /// @brief setter for last_paused_time field
        /// @param last_paused_time new value of last_paused_time
        void set_last_paused_time(time_t);

        /// @brief getter for last_paused_time field
        /// @return last_paused_time value
        time_t get_last_paused_time();

    public:

        /// @brief create new timeline based on anchor and tic
        /// @param anchor anchor timeline to set 
        /// @param tic tic to start timeline with
        LocalTimeline(Timeline&, time_t);

        /// @brief retrieve time based on anchor timeline value
        /// @return timeline time
        time_t get_time();

        /// @brief set paused to true and update last_paused_time
        void pause();

        /// @brief set paused to false
        void unpause();

        /// @brief getter for paused field
        /// @return paused value
        bool get_paused();

        /// @brief setter for tic field
        /// @param tic new value of tic
        void set_tic(time_t);

        /// @brief getter for tic field
        /// @return tic value
        time_t get_tic();

        /// @brief getter for start_time
        /// @return start_time value
        time_t get_start_time();

        /// @brief getter for elapsed_paused_time
        /// @return get_time() - last_paused_time if paused; 0 if !paused
        time_t get_elapsed_paused_time();

    };

}