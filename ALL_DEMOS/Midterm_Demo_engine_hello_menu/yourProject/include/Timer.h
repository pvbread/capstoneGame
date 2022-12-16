#pragma once

#include "pch.h"

class Timer
{
public:
    static Timer* instance();
    static void release();

    void reset();
    float deltaTimer();//gives back delta time

    void timerScale(float t);//for setting timescale
    float timerScale();//returns timescale

    void update();//timer update

    bool timePassed(int countdownTime, int currentTime);

private:
    static Timer* sInstance;

    unsigned int mStartTicks;//where SDL_GETTICKS gets from beginning of program
    unsigned int mElapsedTicks;
    float mDeltaTimer;
    float mTimerScale;

    Timer();
    ~Timer();

};