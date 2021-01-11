#pragma once
#include "DataStruct.h"
#include "WorldMap.h"

class GameManager
{
private:
	GameState state;

	WorldMap worldMap;

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

	void SetWorldMapData(const WorldMap worldMap);
	const WorldMap GetWorldMapData();
};

