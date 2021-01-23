#pragma once
#include "DataStruct.h"
#include "WorldMap.h"
#include "Player.h"
#include "Time.h"
#include "NPC.h"
#include "Item.h"
#include "Inventory.h"
#include <vector>

extern HWND g_hWnd;
extern HWND g_hInventoryDlg;
constexpr const int INPUT_DELAY = 200;
constexpr const int EVENT_DELAY = 200;

using namespace std;

class InteractionManager;
class GameManager
{
private:
	GameState state;

	Player* player;
	Inventory* inventory;
	NPC* npc;
	vector<Item> FieldItem[STAGE_SIZE];	// 필드 스폰 아이템

	Time* time;

	InteractionManager* interactionManager;

private:
	static GameManager* instance;

	GameManager();
	~GameManager();
private:
	ULONGLONG inputTick;
	ULONGLONG eventTick;

public:
	static GameManager* GetInstance();
	static void ReleaseInstance();

public:
	void Input();
	void Run();

	void test();

	void LimitMoveMent(const DPOINT prevDPos);	// 이동 제한
	void UsePortal();		// 포탈 사용
	void RetouchMoveMent(POINT colliderPos[4]);	// 이동 보정

	POINT GetPlayerPivotMapPoint();
public:
	const GameState GetState();
	void SetState(const GameState state);

	void SetPlayer(Player* player);
	Player* GetPlayer();

	void SetInventory(Inventory* inventory);

	void SetNPC(NPC* npc);
	NPC* GetNPC();

	void AddItem(const int stage, const Item item);
	void DeleteItem(const int stage, const int index);
	const Item GetItem(const int stage, const int index);

	vector<Item> GetFieldItem(const int stage);

};

