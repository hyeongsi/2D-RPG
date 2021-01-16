#pragma once
#include "DataStruct.h"
#include "WorldMap.h"

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
	void ChangeMapData(WorldMap* worldMap, POINT pos, const int dir);
};

