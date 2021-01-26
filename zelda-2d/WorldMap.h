#pragma once
#include "DataStruct.h"
#include <vector>

using namespace std;

class WorldMap
{
private:
	int backgroundData[MAP_MAX_Y][MAP_MAX_X]{ 0 };	// 배경 정보
	int objectData[MAP_MAX_Y][MAP_MAX_X]{ 0 };		// 오브젝트 정보
	int coliderData[MAP_MAX_Y][MAP_MAX_X]{ 0 };		// 콜라이더 정보
	int eventData[MAP_MAX_Y][MAP_MAX_X]{ 0 };		// 이벤트 정보

	vector<FieldNPC> npcData;		// npc 정보
public:
	friend class MapEdittor;

	int GetData(SelectMapState state, POINT pos);
	void SetData(SelectMapState state, POINT pos, int data);

	vector<FieldNPC>* GetNPCData();
	void SetNPCData(const FieldNPC fieldNPC);
};

