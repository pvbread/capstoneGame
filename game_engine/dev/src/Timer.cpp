#include "Timer.h"

Timer* Timer::sInstance = nullptr;//dont do this way Timer::sInstance

Timer* Timer::Instance() 
{
    if(sInstance == nullptr)
        sInstance = new Timer();

    return sInstance;
}

void Timer::Release()
{
    delete sInstance;
    sInstance = nullptr;
}

Timer::Timer()
{
    Reset();
    mTimerScale = 1.0f;
}

Timer::~Timer()
{

}

void Timer::Reset()
{
    mStartTicks = SDL_GetTicks();
    mElapsedTicks = 0;
    mDelataTime = 0.0f;
}

float Timer::DeltaTimer()
{
    return mDeltaTimer;
}

void Timer::TimerScale(float t)
{
    mTimerScale = t;
}

float Timer::TimerScale()
{
    return mTimerScale;
}

void Timer::Update()
{
    mElapsedTicks = SDL_GetTicks() - mStartTicks;
    mDeltaTimer = mElapsedTicks = 0.001f;
}