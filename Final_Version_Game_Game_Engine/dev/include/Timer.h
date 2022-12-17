#pragma once

#include "pch.h"

/// @brief general timer class. We use it for transitions
/// effects and also timed notifications.
class Timer
{
public:
    /// @brief This is a singleton so we don't provide direct access to
    /// the timer constructor.
    /// @return 
    static Timer* instance();
    
    static void release();

    void reset();
    
    /// @brief
    /// @return the change in time 
    float deltaTime();

    void timerScale(float t);

    float getTimerScale();

    /// @brief updates the timer (needs to be at the top of the game loop)
    void update();

    /// @brief Checks whether some time has elapsed. Incomplete function.
    /// @param countdownTime 
    /// @param currentTime 
    /// @return 
    bool timePassed(int countdownTime, int currentTime);

private:
    static Timer* sInstance;

    unsigned int mStartTicks;
    unsigned int mElapsedTicks;
    float mDeltaTime;
    float mTimerScale;

    Timer();
    ~Timer();

};