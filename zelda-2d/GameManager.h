#pragma once
#include "DataStruct.h"
#include "WorldMap.h"
#include "Player.h"
#include "Timmer.h"
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

private:
	static GameManager* instance;

	GameManager();
	~GameManager();

private:
	ULONGLONG eventTick;
public:
	static GameManager* GetInstance();
	static void ReleaseInstance();

public:
	void Init();
	void Run();

	void LimitMoveMent(const DPOINT prevDPos);	// 이동 제한
	void PickUpItem();							// 아이템 파밍
	void UsePortal();							// 포탈 사용
	void RetouchMoveMent(POINT colliderPos[4]);	// 이동 보정

	void AttackMonster();						// 공격

	void PushOutPlayer(const int dir);	// 플레이어 밀려남 처리

	POINT GetPlayerPivotMapPoint();
public:
	const GameState GetState();
	void SetState(const GameState state);

	void SetPlayer(Player* player);
	Player* GetPlayer();

	void SetInventory(Inventory* inventory);
	Inventory* GetInventory();
};

