#include "Timer.h"

Timer* Timer::sInstance = nullptr; //what does this do???

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
    release();
}

void Timer::reset()
{
    mStartTicks = SDL_GetTicks();
    mElapsedTicks = 0;
    mDeltaTime = 0.0f;
}

float Timer::deltaTime()
{
    //timer getter to return deltaTimer
    return mDeltaTime;
}

void Timer::timerScale(float t)
{
    mTimerScale = t;
}

float Timer::getTimerScale()
{
    return mTimerScale;
}

void Timer::update()
{
    mElapsedTicks = SDL_GetTicks() - mStartTicks;
    mDeltaTime = mElapsedTicks * 0.001f;//converts back to seconds
}

bool Timer::timePassed(int countdownTime, int currentTime)
{
    // unfinished function. These should probably be floats
    if(currentTime >= currentTime + countdownTime){
        return true;
    }
    return false;
}