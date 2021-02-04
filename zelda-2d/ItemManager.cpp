#include "pch.h"
#include "ItemManager.h"
#include <fstream>

using namespace std;

ItemManager* ItemManager::instance = nullptr;

ItemManager::ItemManager()
{

}

void ItemManager::Init()
{
	fieldItem.clear();
}

ItemManager* ItemManager::GetInstance()
{
	if (nullptr == instance)
		instance = new ItemManager();
	return instance;
}

void ItemManager::ReleaseInstance()
{
	delete instance;
	instance = nullptr;
}

void ItemManager::AddFieldItem(const POINT pos, const int index)
{
	if (static_cast<int>(itemData.size()) < index)
		return;
	if (0 >= index)
		return;

	FieldItem fitem;
	fitem.pos = pos;
	fitem.index = index;

	fieldItem.emplace_back(fitem);
}

vector<FieldItem>* ItemManager::GetFieldItem()
{
	return &fieldItem;
}

vector<Item>* ItemManager::GetItemData()
{
	return &itemData;
}

void ItemManager::LoadItemData()
{
	ifstream readFile;
	string str;
	Item item;

	try
	{
		readFile.open(ITEM_INFO_PATH);
		if (readFile.is_open())
		{
			while (!readFile.eof())
			{
				readFile >> str;
				item.index = stoi(str);
				readFile >> str;
				item.price = stoi(str);
				readFile >> str;
				item.title = str;
				readFile >> str;
				item.explain = str;

				itemData.emplace_back(item);
			}
		}
	}
	catch (const std::exception&)
	{
	}

	readFile.close();
}
