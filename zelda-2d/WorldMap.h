#pragma once
#include "DataStruct.h"

class WorldMap
{
private:
	int backgroundData[MAP_MAX_Y][MAP_MAX_X]{ 0 };	// 배경 정보
	int objectData[MAP_MAX_Y][MAP_MAX_X]{ 0 };		// 오브젝트 정보
	int coliderData[MAP_MAX_Y][MAP_MAX_X]{ 0 };		// 콜라이더 정보
	int eventData[MAP_MAX_Y][MAP_MAX_X]{ 0 };		// 이벤트 정보

	bool isLoad{false};		// 로드 여부 검사
public:
	friend class MapEdittor;

	int GetData(MapEdittorSelectState state, POINT pos);
	void SetData(MapEdittorSelectState state, POINT pos, int data);

	const bool GetIsLoad();
	void SetIsLoad();	

	const POINT ChangePosToMapPoint(const POINT pos);		// 좌표값을 맵 배열에서 사용하는 값으로 변환
};

