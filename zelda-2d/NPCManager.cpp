#include "pch.h"
#include "NPCManager.h"
#include <fstream>

NPCManager* NPCManager::instance = nullptr;

NPCManager::NPCManager()
{
}

NPCManager::~NPCManager()
{
}

NPCManager* NPCManager::GetInstance()
{
	if (nullptr == instance)
		instance = new NPCManager();

	return instance;
}

void NPCManager::ReleaseInstance()
{
	delete instance;
	instance = nullptr;
}

void NPCManager::LoadNPCData()
{
	ifstream readFile;
	string str;

	const int pathSize = 1;
	string path[pathSize];

	try
	{	// npc 파일 전체 경로 로드
		readFile.open(DATA_PATH);
		if (readFile.is_open())
		{
			for (int i = 0; i < pathSize; i++)
			{
				readFile >> str;
				path[i] = str;
			}

			readFile.close();
		}
		else
		{
			readFile.close();
			return;
		}


		// 개별 파일 정보 로드
		for (int i = 0; i < pathSize; i++)
		{
			switch (i)
			{
			case 0: LoadShopNPCData(path[i]);
				break;
			default:
				return;
			}
		}
	}
	catch (const std::exception&)
	{
		readFile.close();
	}
}

void NPCManager::LoadShopNPCData(const string path)
{
	ifstream readFile;
	string str;
	string sellItemSize;

	const int explainStringSize = 3;	// id, sellItem, Size

	try
	{
		readFile.open(path);
		if (readFile.is_open())
		{	
			for (int i = 0; i < explainStringSize; i++)
				readFile >> str;	
			

			while (!readFile.eof())
			{
				ShopNPC shopNPC;

				readFile >> str;	// id
				readFile >> sellItemSize;	// sellItemSize

				for (int i = 0; i < stoi(sellItemSize); i++)
				{
					readFile >> str;
					shopNPC.SetSellItemId(stoi(str));
				}
			}
			readFile.close();
		}
		else
		{
			readFile.close();
			return;
		}
	}
	catch (const std::exception&)
	{
		readFile.close();
	}
}
