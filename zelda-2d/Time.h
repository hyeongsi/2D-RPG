#pragma once
class Time
{
private:
	static Time* instance;

	Time();
	~Time();

private:
	LARGE_INTEGER countTime;
	LARGE_INTEGER curTime;
	LARGE_INTEGER prevTime;

	double deltaTime;

public:
	void Reset();
	const double Update();

public:
	static Time* GetInstance();
	static void ReleaseInstance();

};

