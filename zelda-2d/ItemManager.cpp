#include "pch.h"
#include "ItemManager.h"
#include <fstream>

using namespace std;

ItemManager* ItemManager::instance = nullptr;

ItemManager::ItemManager()
{

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
