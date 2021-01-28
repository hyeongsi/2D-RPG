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
	void ActionEvent(POINT pos);

	void OpenWoodHouseDoor(const POINT pos);	// 오두막 문여는 이벤트
	void CloseWoodHouseDoor(const POINT pos);	// 오두막 문닫는 이벤트
	void DropItem(const POINT pos);	// 아이템 드랍 이벤트

	void UseItem();					// 아이템 사용 이벤트
	void BuyItem();					// 아이템 구매 이벤트
	const int FindInventoryItemIndex();	// 인벤 아이템 인덱스 찾기
	const int FindBuyItemId();		// 구매 아이템 인덱스 찾기

	void SwitchInventoryItem(const int preIndex);		// 인벤 아이템 스위칭 이벤트
};
