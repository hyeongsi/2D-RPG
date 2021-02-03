#include "pch.h"
#include "Timmer.h"

Timmer* Timmer::instance = nullptr;

Timmer::Timmer()
{
	Reset();
}

Timmer::~Timmer()
{
}

void Timmer::Reset()
{
	QueryPerformanceFrequency(&countTime);
	QueryPerformanceCounter(&curTime);
	QueryPerformanceCounter(&prevTime);
}

const double Timmer::Update()
{
	QueryPerformanceCounter(&curTime);

	deltaTime = (static_cast<double>(curTime.QuadPart) - static_cast<double>(prevTime.QuadPart))
		/ static_cast<double>(countTime.QuadPart);

	prevTime = curTime;

	return deltaTime;
}

Timmer* Timmer::GetInstance()
{
	if (nullptr == instance)
		instance = new Timmer();

	return instance;
}

void Timmer::ReleaseInstance()
{
	delete instance;
	instance = nullptr;
}
