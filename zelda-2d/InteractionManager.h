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
	/*void ChangeMapData(POINT pos);
	void ActionEvent(const POINT pos);*/

};
