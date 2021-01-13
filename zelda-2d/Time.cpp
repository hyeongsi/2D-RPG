#include "pch.h"
#include "Time.h"

Time* Time::instance = nullptr;

Time::Time()
{
	Reset();
}

Time::~Time()
{
}

void Time::Reset()
{
	QueryPerformanceFrequency(&countTime);
	QueryPerformanceCounter(&curTime);
	QueryPerformanceCounter(&prevTime);
}

const double Time::Update()
{
	QueryPerformanceCounter(&curTime);

	deltaTime = (static_cast<double>(curTime.QuadPart) - static_cast<double>(prevTime.QuadPart))
		/ static_cast<double>(countTime.QuadPart);

	prevTime = curTime;

	return deltaTime;
}

Time* Time::GetInstance()
{
	if (nullptr == instance)
		instance = new Time();

	return instance;
}

void Time::ReleaseInstance()
{
	delete instance;
	instance = nullptr;
}
