#pragma once
#include "DataStruct.h"
#include "WorldMap.h"
#include "Character.h"
#include "Time.h"
#include "InteractionManager.h"

class GameManager
{
private:
	GameState state;

	WorldMap worldMap;

	Character* character;

	Time* time;

	InteractionManager* interactionManager;

private:
	static GameManager* instance;

	GameManager();
	~GameManager();

public:
	static GameManager* GetInstance();
	static void ReleaseInstance();

public:
	void Run();
	void LimitMoveMent(const DPOINT prevDPos);			
public:
	const GameState GetState();
	void SetState(const GameState state);

	void SetWorldMapData(const WorldMap worldMap);
	const WorldMap GetWorldMapData();

	void SetCharacter(Character* character);
	Character* GetCharacter();

};

