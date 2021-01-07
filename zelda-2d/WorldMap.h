#pragma once
#include "DataStruct.h"

class WorldMap
{
private:
	int backgroundData[MAP_MAX_Y][MAP_MAX_X]{ 0 };	// 배경 정보
	int objectData[MAP_MAX_Y][MAP_MAX_X]{ 0 };		// 오브젝트 정보
	int coliderData[MAP_MAX_Y][MAP_MAX_X]{ 0 };		// 콜라이더 정보

public:
	friend class MapEdittor;

	int GetData(MapEdittorSelectState state, POINT pos);
	void SetData(MapEdittorSelectState state, POINT pos, int data);
};

