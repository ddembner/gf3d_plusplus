#include "gf3d_time.hpp"
#include <chrono>
#include "math/math_functions.hpp"

void Time::init()
{
	using namespace std::chrono;

	auto now = high_resolution_clock::now().time_since_epoch();
	mOffset = now.count();
}

void Time::update()
{
	using namespace std::chrono;
	f64 now = static_cast<f64>(high_resolution_clock::now().time_since_epoch().count() - mOffset) / 1000000000;
	mDeltaTime = static_cast<f64>(gf3d::min((now - mUnscaledTime) * timeScale, 0.1f)); // cap deltaTime to 0.1
	mTime += mDeltaTime;
	mUnscaledTime = now;
	mFrameCount++;
}
