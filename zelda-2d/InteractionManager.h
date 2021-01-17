#pragma once
#include "DataStruct.h"
#include "WorldMap.h"
#include <vector>

using namespace std;

class InteractionManager
{
private:
	static InteractionManager* instance;

	InteractionManager();
	~InteractionManager();
public:
	static InteractionManager* GetInstance();
	static void ReleaseInstance();

public:
	void ChangeMapData(POINT pos);
	void ActionEvent(const POINT pos);

public:
	void LoadTextMapData(const GameState state, const char* filePath);           // 맵 정보 로드
	void LoadTextEventData(const char* filePath, WorldMap* worldMap);           // 맵과 관련된 이벤트 정보 로드

};
