﻿#pragma once
#include "DataStruct.h"
#include "WorldMap.h"
#include "Character.h"
#include "Time.h"

class GameManager
{
private:
	GameState state;

	WorldMap worldMap;

	Character* character;

	Time* time;

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

	void SetCharacter(Character* character);
	const Character GetCharacter();
};

