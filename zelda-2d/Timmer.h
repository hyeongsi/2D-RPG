#pragma once
class Timmer
{
private:
	static Timmer* instance;

	Timmer();
	~Timmer();

private:
	LARGE_INTEGER countTime;
	LARGE_INTEGER curTime;
	LARGE_INTEGER prevTime;

public:
	double deltaTime;

	void Reset();
	const double Update();

public:
	static Timmer* GetInstance();
	static void ReleaseInstance();

};

