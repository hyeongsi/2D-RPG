#pragma once
#include "DataStruct.h"

class GameManager
{
private:
	GameState state;
private:
	static GameManager* instance;

	GameManager();
	~GameManager();

public:
	static GameManager* GetInstance();
	static void ReleaseInstance();

public:
	void Run();
public:
	const GameState GetState();
	void SetState(const GameState state);
};

