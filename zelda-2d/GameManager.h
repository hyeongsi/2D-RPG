#pragma once
#include "DataStruct.h"
#include "WorldMap.h"
#include "Player.h"
#include "Time.h"
#include "ShopNPC.h"
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
	ShopNPC* npc;

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

	void LimitMoveMent(const DPOINT prevDPos);	// 이동 제한
	void PickUpItem();							// 아이템 파밍
	void UsePortal();							// 포탈 사용
	void RetouchMoveMent(POINT colliderPos[4]);	// 이동 보정

	POINT GetPlayerPivotMapPoint();
public:
	const GameState GetState();
	void SetState(const GameState state);

	void SetPlayer(Player* player);
	Player* GetPlayer();

	void SetInventory(Inventory* inventory);
	Inventory* GetInventory();

	void SetNPC(ShopNPC* npc);
	ShopNPC* GetNPC();
};

