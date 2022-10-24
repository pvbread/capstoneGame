#pragma once

#include "pch.h"

class Timer
{
public:
    static Timer* Instance();
    static void Release();

    void Reset();
    float DeltaTimer();

    void TimerScale(float t);
    float TimerScale();

    void Update();

private:
    static Timer* sInstance;

    unsigned int mStartTicks;
    unsigned int mElapsedTicks;
    float mDeltaTimer;
    float mTimerScale;

    Timer();
    ~Timer();

};