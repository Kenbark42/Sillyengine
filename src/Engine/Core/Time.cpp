#include "Engine/Core/Time.hpp"
#include "Engine/Core/Logger.hpp"

namespace Engine
{

    Time::Time()
        : deltaTime(0.0f),
          totalTime(0.0f),
          frameCount(0),
          fps(0.0f),
          fpsAccumulator(0.0f),
          fpsFrameAccumulator(0),
          nextTimerId(1)
    {
        reset();
    }

    Time::~Time()
    {
        // Clear timers
        timers.clear();
    }

    void Time::reset()
    {
        startTime = Clock::now();
        lastUpdateTime = startTime;
        deltaTime = 0.0f;
        totalTime = 0.0f;
        frameCount = 0;
        fps = 0.0f;
        fpsAccumulator = 0.0f;
        fpsFrameAccumulator = 0;
    }

    void Time::update()
    {
        // Get current time
        TimePoint currentTime = Clock::now();

        // Calculate delta time (in seconds)
        std::chrono::duration<float> duration = currentTime - lastUpdateTime;
        deltaTime = duration.count();

        // Cap delta time to prevent simulation issues after breakpoints or long pauses
        // 0.2 seconds = 5 FPS minimum to avoid physics/logic issues
        const float maxDeltaTime = 0.2f;
        if (deltaTime > maxDeltaTime)
        {
            deltaTime = maxDeltaTime;
        }

        // Update total time
        duration = currentTime - startTime;
        totalTime = duration.count();

        // Update frame count
        frameCount++;

        // Update FPS counter (average over 1 second)
        fpsAccumulator += deltaTime;
        fpsFrameAccumulator++;

        if (fpsAccumulator >= 1.0f)
        {
            fps = static_cast<float>(fpsFrameAccumulator) / fpsAccumulator;
            fpsAccumulator = 0.0f;
            fpsFrameAccumulator = 0;
        }

        // Update timers
        updateTimers(deltaTime);

        // Set last update time
        lastUpdateTime = currentTime;
    }

    uint32_t Time::createTimer(std::function<void()> callback, float delay, bool repeat)
    {
        Timer timer;
        timer.id = nextTimerId++;
        timer.delay = delay;
        timer.timeLeft = delay;
        timer.repeat = repeat;
        timer.callback = callback;

        timers.push_back(timer);
        return timer.id;
    }

    bool Time::cancelTimer(uint32_t id)
    {
        for (auto it = timers.begin(); it != timers.end(); ++it)
        {
            if (it->id == id)
            {
                timers.erase(it);
                return true;
            }
        }

        return false;
    }

    void Time::updateTimers(float deltaTime)
    {
        // Iterate through all timers
        for (auto it = timers.begin(); it != timers.end();)
        {
            // Reduce time left
            it->timeLeft -= deltaTime;

            // Check if timer expired
            if (it->timeLeft <= 0.0f)
            {
                // Execute callback
                try
                {
                    it->callback();
                }
                catch (const std::exception &e)
                {
                    Logger::error("Timer callback exception: " + std::string(e.what()));
                }

                // Handle repeat or removal
                if (it->repeat)
                {
                    // Reset time left for repeating timers
                    it->timeLeft += it->delay;
                    // Handle case where deltaTime > delay
                    while (it->timeLeft <= 0.0f)
                    {
                        it->timeLeft += it->delay;
                    }
                    ++it;
                }
                else
                {
                    // Remove non-repeating timers
                    it = timers.erase(it);
                }
            }
            else
            {
                ++it;
            }
        }
    }

} // namespace Engine