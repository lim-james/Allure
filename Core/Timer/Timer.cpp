#include "Timer.h"

Timer::Timer() 
	: et(0.0)
	, dt(0.0) {
	begin = previous = current = {};
	QueryPerformanceFrequency(&frequency);
}

void Timer::Start() {
	QueryPerformanceCounter(&begin);
	previous = begin;
}

void Timer::Update() {
	LARGE_INTEGER elapsed, delta;
	QueryPerformanceCounter(&current);

	elapsed.QuadPart = current.QuadPart - begin.QuadPart;
	delta.QuadPart = current.QuadPart - previous.QuadPart;

	previous = current;

	et = LargeIntToSecs(elapsed);
	dt = LargeIntToSecs(delta);
}

double const& Timer::GetDeltaTime() const {
	return dt;
}

double const& Timer::GetElapsedTime() const {
	return et;
}

double Timer::LargeIntToSecs(LARGE_INTEGER const& L) const {
	return static_cast<double>(L.QuadPart) / static_cast<double>(frequency.QuadPart);
}
