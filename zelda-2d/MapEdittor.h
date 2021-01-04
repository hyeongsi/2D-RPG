#pragma once
#include "DataStruct.h"
#include "WorldMap.h"

class MapEdittor
{
private:
	int selectBitmap;					// 선택한 비트맵의 번호
	HBITMAP hBitmap;					// 마우스에 출력될 비트맵
	MapEdittorSelectState selectState;	// 배경, 오브젝트, 콜라이더 선택 상태	

	WorldMap mapData;					// 맵 데이터

	HDC hdc{ nullptr };
	HDC memDC{ nullptr };
	HDC backMemDC{ nullptr };
	HBITMAP oldCreateBitmap{ nullptr };
public:
	MapEdittor();
	~MapEdittor();
public:
	void Init();
	void Run();

	void Render();
};

