#pragma once

#include "DataStruct.h"
#include "ImageManager.h"
#include "MapEdittor.h"
#include "GameManager.h"
#include "Character.h"

#pragma comment (lib, "Msimg32.lib")

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
	void InGameDataRender(Character character);

public:
	void DrawWorldMapData(const GameState gameState);

	void DrawCharUIData(const int uiName, const POINT pos);

	void DrawCheckPattern();
	void DrawCursorFollowBitmap();
};

