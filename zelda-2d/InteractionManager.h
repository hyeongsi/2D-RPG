#pragma once
#include "DataStruct.h"
#include "WorldMap.h"
#include <vector>
#include <functional>

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

	void OpenWoodHouseDoor(const POINT pos);	// 오두막 문여는 이벤트
	void CloseWoodHouseDoor(const POINT pos);	// 오두막 문닫는 이벤트
	void DropItem(const POINT pos);	// 아이템 드랍 이벤트
};
