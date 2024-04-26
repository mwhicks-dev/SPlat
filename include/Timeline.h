#include <mutex>

namespace SPlat {

    /// @brief abstract Timeline class
    class Timeline {

        void set_paused(bool);

        void set_last_paused_time(time_t);

    protected:

        Timeline();

        /// @brief access control for mutable variables
        std::mutex m;

        /// @brief time of anchor on creation
        time_t start_time;

        /// @brief for deltat calculation when paused
        time_t elapsed_paused_time;

        /// @brief for deltat calculation when paused
        time_t last_paused_time;

        /// @brief numbers of anchor timeline step per this step
        time_t tic;

        /// @brief bool indicating whether or not timeline paused
        bool paused;

        /// @brief anchor timeline
        Timeline* anchor;

    public:

        /// @brief Timeline constructor
        /// @param anchor anchor Timeline 
        /// @param tic time_t tic to set
        Timeline(Timeline&, time_t);

        /// @brief gets (game or system) time implementation
        /// @return current timeline time
        virtual time_t get_time();

        /// @brief set paused to true; set last_paused_time
        void pause();

        /// @brief set paused to false
        void unpause();

        /// @brief change tic
        /// @param tic new tic to set
        void set_tic(time_t);

        /// @brief return whether or not currently paused
        /// @return whether or not currently paused
        bool get_paused();

        time_t get_start_time();

        time_t get_tic();

    };

    /// @brief singleton system time based Timeline
    class SystemTimeline : public Timeline {

        /// @brief no anchor timeline and tic is 1 (own anchor)
        SystemTimeline() = default;

    public:

        time_t get_time() override;

        /// @brief retrieve singleton system time
        /// @return singleton SystemTimeline instance
        static SystemTimeline& get_instance() {
            static SystemTimeline instance;
            return instance;
        }

    };

}