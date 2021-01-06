#pragma once
#include "DataStruct.h"
#include "WorldMap.h"
#include "resource.h"
#include <map>

using namespace std;

class MapEdittor
{
private:
	int selectBitmap;					// 선택한 비트맵의 번호
	HBITMAP hBitmap;					// 마우스에 출력될 비트맵
	MapEdittorSelectState selectState;	// 배경, 오브젝트, 콜라이더 선택 상태	

	WorldMap mapData;					// 맵 데이터

	map<int, HBITMAP> backgroundBitmapData;		// 배경 비트맵
	map<int, HBITMAP> objectBitmapData;			// 오브젝트 비트맵

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

public:
	void LoadBitmapData(const map<int, HBITMAP> backgroundBitmapData, const map<int, HBITMAP> objectBitmapData);
	void SetMapData(const POINT pos, const bool isLbutton);

	
};
