#include "pch.h"
#include "NPCManager.h"
#include "WorldMapManager.h"
#include <fstream>

NPCManager* NPCManager::instance = nullptr;

NPCManager::NPCManager()
{
	InitInteractNPCInfo();
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

				shopNPCVector.emplace_back(shopNPC);
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

void NPCManager::InteractNPC(const POINT pos)
{
	for (const auto& iterator : (*WorldMapManager::GetInstance()->GetWorldMap()->GetNPCData()))
	{
		if (!(iterator.pos.x == pos.x && iterator.pos.y + 1 == pos.y))
			continue;

		// npc 위치와 이벤트 위치가 동일하다면
		switch (iterator.kind)
		{
		case SHOP_NPC:
			interactNPCInfo.state = InteractNPCState::SHOP_NPC;
			interactNPCInfo.index = iterator.index;
			break;
		default:
			interactNPCInfo.state = InteractNPCState::NONE;
			break;
		}	
	}
}

vector<ShopNPC>* NPCManager::GetshopNPCVector()
{
	return &shopNPCVector;
}

const InteractNPCInfo NPCManager::GetInteractNPCData()
{
	return interactNPCInfo;
}

void NPCManager::SetInteractNPCData(const InteractNPCInfo interactNPCInfo)
{
	this->interactNPCInfo = interactNPCInfo;
}

void NPCManager::InitInteractNPCInfo()
{
	InteractNPCInfo interactNPCInfoData;
	interactNPCInfoData.state = InteractNPCState::NONE;
	interactNPCInfoData.index = 0;
	interactNPCInfo = interactNPCInfoData;
}
