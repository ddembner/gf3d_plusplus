#pragma once
#include "defines.hpp"

class Time
{
public:
	Time() : mUnscaledTime(0), mTime(0), mDeltaTime(0), mFrameCount(0), mOffset(0), timeScale(1.0) { }

	f64 unscaledTime() const { return mUnscaledTime; }
	f64 time() const { return mTime; }
	f64 deltaTime() const { return mDeltaTime; }
	u64 frameCount() const { return mFrameCount; }
	void init();
	void update();

public:
	f64 timeScale;
private:
	f64 mUnscaledTime;
	f64 mTime; // time in seconds
	f64 mDeltaTime;
	u64 mFrameCount;
	u64 mOffset; // the start time of the application in nanoseconds
};