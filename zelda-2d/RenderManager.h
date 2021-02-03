#pragma once

#include "DataStruct.h"
#include "ImageManager.h"
#include "MapEdittor.h"
#include "GameManager.h"
#include "Item.h"
#include "Player.h"
#include "ShopNPC.h"
#include <vector>
#include <string>

using namespace std;

#pragma comment (lib, "Msimg32.lib")

constexpr const POINT INVENTORY_SPAWN_POS = { 600,170 };
constexpr const SIZE INVENTORY_interval_SIZE = { 8,8 };

constexpr const POINT DRAW_PLAYERINFO_UI_POS = { 10,10 };
constexpr const POINT DRAW_MONEYINFO_UI_POS = { 11,110 };

constexpr const POINT SHOP_SPAWN_POS = { 200,170 };
constexpr const SIZE SHOP_SIZE = { 230,260 };
constexpr const SIZE SHOP_ITEMBOX_SIZE = { 200,42 };
constexpr const SIZE SHOP_INTERVAL_SIZE = { 15,5 };

constexpr const int SELL_ITEM_SIZE = 5;
constexpr const int HP_UI_COUNT = 3;

typedef struct HUDData
{
	DPOINT pos;
	string msg;
	ULONGLONG tick;
	COLORREF color = NULL;
}hudData;

class RenderManager
{
private:
	HDC hdc{ nullptr };
	HDC memDC{ nullptr };
	HDC backMemDC{ nullptr };
	HBITMAP oldCreateBitmap{ nullptr };
	
	vector<hudData> hudStringVector;	// hud 출력 문자열 저장 변수

private:
	const int PRINT_HUD_TIME = 1000;
	const int HUD_SPEED = 20;
private:
	static RenderManager* instance;

	RenderManager();
	~RenderManager();
public:
	RECT SHOP_ITEM_BOX_POS[SELL_ITEM_SIZE];
	RECT INVENTORY_ITEM_POS[INVEN_SIZE];

	static RenderManager* GetInstance();
	static void ReleaseInstance();

public:
	void Init();
	void RenderInitSetting();

	void InitHudStringVector();	// hud 초기화
	void AddHudStringVector(hudData data);	// hud 추가
	void DeleteEndHud();	// hud 삭제
	void RisingHud();	// hud 이동
public:
	void SaveMemDcData(const HWND itemHwnd, POINT pos);
	void SaveMemDcData(const HBITMAP hbitmap, POINT pos);
	void Render();
	
public:
	void MainFrameDataRender();
	void MapEdittorDataRender();
	void InGameDataRender();

public:
	void DrawWorldMapData(const GameState gameState);
	void DrawFieldItem();		// 필드 아이템 그리기
	void DrawInventoryItem();	// 인벤토리 아이템 그리기
	void DrawInvenItemExplain();	// 인벤 창 아이템 설명 그리기

	void DrawPlayer();			
	void DrawNPC();				// NPC 출력
	void DrawMonster();			// 적 출력
	void DrawNpcOrderPos();		// 출력 순서에 따른 NPC 출력
	void DrawMonsterOrderPos();	// 출력 순서에 따른 NPC 출력
	void DrawShop();			// 상점 출력
	void DrawCharUIData(const int uiName, const POINT pos);

	void DrawHud(const DPOINT pos, const string msg);
	void DrawHudVector();

	void DrawCheckPattern(HDC hdc, const SIZE size);
	void DrawCursorFollowBitmap();

public:
	void DrawPlayerAnimation(const int uiName, const DPOINT pos);
	void InitPlayerAnimation(const int state);

	void DrawMonsterAnimation(const int index);

};

