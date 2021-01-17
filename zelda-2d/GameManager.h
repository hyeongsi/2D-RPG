#pragma once
#include "DataStruct.h"
#include "WorldMap.h"
#include "Player.h"
#include "Time.h"
#include "NPC.h"

constexpr const int TICK_DELAY = 250;

using namespace std;

class InteractionManager;
class GameManager
{
private:
	GameState state;

	WorldMap worldMap[STAGE_SIZE];	// stage 1(길거리), 2(오두막)
	int currentStage;		// 현재 스테이지

	Player* player;
	NPC* npc;

	Time* time;

	InteractionManager* interactionManager;

private:
	static GameManager* instance;

	GameManager();
	~GameManager();
private:
	ULONGLONG tick;
public:
	static GameManager* GetInstance();
	static void ReleaseInstance();

public:
	void Run();
	void LimitMoveMent(const DPOINT prevDPos);			
public:
	const GameState GetState();
	void SetState(const GameState state);

	void SetWorldMapData(const WorldMap worldMap, const int index);
	WorldMap* GetWorldMapData(const int index);

	void SetCurrentStage(const int currentStage);
	const int GetCurrentStage();

	void SetPlayer(Player* player);
	Player* GetPlayer();

	void SetNPC(NPC* npc);
	NPC* GetNPC();
};

