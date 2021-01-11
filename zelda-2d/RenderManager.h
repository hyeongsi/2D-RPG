#pragma once
#include "DataStruct.h"

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
	void InGameDataRender();

public:
	void DrawWorldMapData(const GameState gameState);
	void DrawCheckPattern();
	void DrawCursorFollowBitmap();
};

