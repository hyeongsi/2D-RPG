#pragma once
#include "DataStruct.h"

class GameManager
{
private:
	GameState state;

public:
	void Run();
public:
	GameManager();

	const GameState GetState();
	void SetState(const GameState state);
};

