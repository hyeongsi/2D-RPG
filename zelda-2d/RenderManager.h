#pragma once

#include "DataStruct.h"
#include "ImageManager.h"
#include "MapEdittor.h"
#include "GameManager.h"
#include "Item.h"
#include "Player.h"
#include "ShopNPC.h"
#include <vector>

using namespace std;

#pragma comment (lib, "Msimg32.lib")

constexpr const POINT INVENTORY_SPAWN_POS = { 600,170 };
constexpr const SIZE INVENTORY_interval_SIZE = { 8,8 };
constexpr const POINT SHOP_SPAWN_POS = { 200,170 };
constexpr const SIZE SHOP_SIZE = { 230,260 };
constexpr const SIZE SHOP_ITEMBOX_SIZE = { 200,42 };
constexpr const SIZE SHOP_INTERVAL_SIZE = { 15,5 };
constexpr const int SELL_ITEM_SIZE = 5;

constexpr int HP_UI_COUNT = 3;

class RenderManager
{
private:
	HDC hdc{ nullptr };
	HDC memDC{ nullptr };
	HDC backMemDC{ nullptr };
	HBITMAP oldCreateBitmap{ nullptr };
	
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
	void DrawNPC();
	void DrawNpcOrderPos();		// 출력 순서에 따른 NPC 출력
	void DrawShop();			// 상점 출력
	void DrawCharUIData(const int uiName, const POINT pos);

	void DrawCheckPattern(HDC hdc, const SIZE size);
	void DrawCursorFollowBitmap();

public:
	void DrawAnimation(const int uiName, const DPOINT pos);

	void InitPlayerAnimation(const int state);

};

