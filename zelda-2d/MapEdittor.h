#pragma once
#include "DataStruct.h"
#include "WorldMap.h"
#include "resource.h"
#include <map>

using namespace std;

class MapEdittor
{
private:
	int selectIndex;					// 선택한 비트맵 인덱스
	SelectMapState selectState;	// 배경, 오브젝트, 콜라이더 선택 상태	

	WorldMap* mapData{ nullptr };					// 맵 데이터
private:
	static MapEdittor* instance;

	MapEdittor();
	~MapEdittor();

public:
	static MapEdittor* GetInstance();
	static void ReleaseInstance();

public:
	void Init();

public:
	void SetMapData(const POINT pos, const bool isLbutton);

	const SelectMapState GetSelectState();
	void SetSelectState(const SelectMapState state);

	const WorldMap GetWorldMapData();
	void SetWorldMapData(const WorldMap data);

	const int GetSelectIndex();
	void SetSelectIndex(const int index);
};
