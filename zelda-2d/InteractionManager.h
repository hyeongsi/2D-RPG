#pragma once
#include "DataStruct.h"
#include "WorldMap.h"
#include <vector>

using namespace std;

constexpr const int tickDelay = 250;

class InteractionManager
{
private:
	static InteractionManager* instance;

	InteractionManager();
	~InteractionManager();
private:
	ULONGLONG tick;
public:
	static InteractionManager* GetInstance();
	static void ReleaseInstance();

public:
	void ChangeMapData(WorldMap* worldMap, POINT pos, const int dir);
	void ActionEvent(WorldMap* worldMap, const POINT pos);
};

