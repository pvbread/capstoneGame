#include "Timer.h"

Timer* Timer::sInstance = nullptr;//dont do this way Timer::sInstance

Timer* Timer::instance() 
{
    if(sInstance == nullptr)
        sInstance = new Timer();

    return sInstance;
}

void Timer::release()
{
    //releases our current instance
    delete sInstance; 
    sInstance = nullptr;
}

Timer::Timer()
{
    //timer constructor. Sets the scale to 1 millisecond
    reset();
    mTimerScale = 1.0f;
}

Timer::~Timer()
{
    //timer deconstructor
}

void Timer::reset()
{
    mStartTicks = SDL_GetTicks();
    mElapsedTicks = 0;
    mDelataTime = 0.0f;
}

float Timer::deltaTimer()
{
    //timer getter to return deltaTimer
    return mDeltaTimer;
}

void Timer::timerScale(float t)
{
    mTimerScale = t;
}

float Timer::timerScale()
{
    return mTimerScale;
}

void Timer::update()
{
    mElapsedTicks = SDL_GetTicks() - mStartTicks;
    mDeltaTimer = mElapsedTicks * 0.001f;//converts back to seconds
}

bool Timer::timePassed(int countdownTime, int currentTime)
{
    if(currentTime >= currentTime + countdownTime){
        return true;
    }
    //will be used to count down from a time
    //need to check if instance of when timer starts will be equal to timer start + countdown
    //when it is return true
    //!!!!!NO FOR OR WHILE LOOPS!!!!
}