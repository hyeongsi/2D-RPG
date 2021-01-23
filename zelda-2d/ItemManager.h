#pragma once
#include "Item.h"
#include <vector>

using namespace std;

constexpr const char ITEM_INFO_PATH[] = { "data/itemInfo/ItemInfo.txt" };

class ItemManager
{
private:
	vector<Item> itemData;

private:
	static ItemManager* instance;

	ItemManager();
public:
	static ItemManager* GetInstance();
	static void ReleaseInstance();

public:
	void LoadItemData();
};

