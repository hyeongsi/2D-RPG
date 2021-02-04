#pragma once
#include "Item.h"
#include <vector>

using namespace std;

constexpr const char ITEM_INFO_PATH[] = { "data/itemInfo/ItemInfo.txt" };

typedef struct FieldItem
{
	POINT pos;
	int index;
}FieldItem;

class ItemManager
{
private:
	vector<Item> itemData;
	vector<FieldItem> fieldItem;

private:
	static ItemManager* instance;

	ItemManager();
public:
	void Init();

	static ItemManager* GetInstance();
	static void ReleaseInstance();
public:
	void AddFieldItem(const POINT pos, const int index);
	vector<FieldItem>* GetFieldItem();

	vector<Item>* GetItemData();
public:
	void LoadItemData();
};

