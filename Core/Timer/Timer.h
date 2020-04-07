#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

class Timer {

	LARGE_INTEGER frequency;
	LARGE_INTEGER begin, previous, current;

	double et, dt;

public:

	Timer();

	void Start();
	void Update();

	double const& GetDeltaTime() const;
	double const& GetElapsedTime() const;

private:

	double LargeIntToSecs(LARGE_INTEGER const& L) const;

};

#endif