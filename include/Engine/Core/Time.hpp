#pragma once

#include <chrono>
#include <vector>
#include <functional>

namespace Engine
{

    /**
     * @brief Time management class
     *
     * The Time class provides utilities for tracking time-related information
     * such as delta time, elapsed time, and managing timers.
     */
    class Time
    {
    public:
        /**
         * @brief Constructor
         */
        Time();

        /**
         * @brief Destructor
         */
        ~Time();

        /**
         * @brief Updates the time state
         */
        void update();

        /**
         * @brief Resets the time state
         */
        void reset();

        /**
         * @brief Gets the time elapsed since the last update
         * @return Delta time in seconds
         */
        float getDeltaTime() const { return deltaTime; }

        /**
         * @brief Gets the total time elapsed since the start or last reset
         * @return Total time in seconds
         */
        float getTotalTime() const { return totalTime; }

        /**
         * @brief Gets the frame count since the start or last reset
         * @return Frame count
         */
        uint64_t getFrameCount() const { return frameCount; }

        /**
         * @brief Gets the current frames per second
         * @return Frames per second
         */
        float getFPS() const { return fps; }

        /**
         * @brief Creates a timer that executes a callback after a specified time
         * @param callback Function to call when the timer expires
         * @param delay Delay in seconds
         * @param repeat True if the timer should repeat, false otherwise
         * @return Timer ID
         */
        uint32_t createTimer(std::function<void()> callback, float delay, bool repeat = false);

        /**
         * @brief Cancels a timer
         * @param id Timer ID
         * @return True if the timer was cancelled, false if it doesn't exist
         */
        bool cancelTimer(uint32_t id);

    private:
        /**
         * @brief Timer structure
         */
        struct Timer
        {
            uint32_t id;
            float delay;
            float timeLeft;
            bool repeat;
            std::function<void()> callback;
        };

        /**
         * @brief Updates all timers
         * @param deltaTime Time elapsed since the last update
         */
        void updateTimers(float deltaTime);

        /**
         * @brief Clock type
         */
        using Clock = std::chrono::high_resolution_clock;

        /**
         * @brief Time point type
         */
        using TimePoint = std::chrono::time_point<Clock>;

        /**
         * @brief Last update time point
         */
        TimePoint lastUpdateTime;

        /**
         * @brief Start time point
         */
        TimePoint startTime;

        /**
         * @brief Delta time in seconds
         */
        float deltaTime;

        /**
         * @brief Total time in seconds
         */
        float totalTime;

        /**
         * @brief Frame count
         */
        uint64_t frameCount;

        /**
         * @brief Current frames per second
         */
        float fps;

        /**
         * @brief FPS calculation time accumulator
         */
        float fpsAccumulator;

        /**
         * @brief FPS calculation frame accumulator
         */
        uint32_t fpsFrameAccumulator;

        /**
         * @brief List of active timers
         */
        std::vector<Timer> timers;

        /**
         * @brief Next timer ID
         */
        uint32_t nextTimerId;
    };

} // namespace Engine