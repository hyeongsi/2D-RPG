#pragma once
#include "DataStruct.h"
#include "WorldMap.h"
#include "Player.h"
#include "Time.h"
#include "NPC.h"
#include "Item.h"
#include <vector>

extern HWND g_hWnd;
extern HWND g_hInventoryDlg;
constexpr const int EVENT_DELAY = 250;

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
	vector<Item> FieldItem[STAGE_SIZE];	// 필드 스폰 아이템

	Time* time;

	InteractionManager* interactionManager;

private:
	static GameManager* instance;

	GameManager();
	~GameManager();
private:
	ULONGLONG eventDelay;

public:
	static GameManager* GetInstance();
	static void ReleaseInstance();

public:
	void Input();
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

	void AddItem(const int stage, const Item item);
	void DeleteItem(const int stage, const int index);
	const Item GetItem(const int stage, const int index);

	vector<Item> GetFieldItem(const int stage);

};

