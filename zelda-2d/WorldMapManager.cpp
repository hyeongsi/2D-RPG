#include "pch.h"
#include "WorldMapManager.h"
#include "MapEdittor.h"
#include "GameManager.h"
#include <fstream>

WorldMapManager* WorldMapManager::instance = nullptr;

WorldMapManager::WorldMapManager()
{
	currentStage = 0;
	LoadPathData();			// 맵 경로 관련 데이터 변수에 저장
}

WorldMapManager* WorldMapManager::GetInstance()
{
	if (nullptr == instance)
		instance = new WorldMapManager();
	return instance;
}

void WorldMapManager::ReleaseInstance()
{
	delete instance;
	instance = nullptr;
}

const POINT WorldMapManager::ChangePosToMapPoint(const POINT pos)
{
	POINT mapPos;
	mapPos.x = pos.x / TILE_SIZE;
	mapPos.y = pos.y / TILE_SIZE;

	return mapPos;
}

void WorldMapManager::SetWorldMap(const WorldMap worldmap)
{
	this->worldMap = worldmap;
}

WorldMap* WorldMapManager::GetWorldMap()
{
	return &worldMap;
}

void WorldMapManager::SetCurrentStage(const int currentStage)
{
	this->currentStage = currentStage;
}

const int WorldMapManager::GetCurrentStage()
{
	return currentStage;
}

void WorldMapManager::AddProtalData(const Portal portal)
{
	protalData.emplace_back(portal);
}

const vector<Portal> WorldMapManager::GetProtalData()
{
	return protalData;
}

void WorldMapManager::DeleteProtalData(const int index)
{
	if (index < static_cast<int>(protalData.size()) && 0 <= index)
	{
		auto iterator = protalData.begin();
		for (int i = 0; i < index; i++)
			iterator++;

		protalData.erase(iterator);
	}
}

void WorldMapManager::LoadPathData()
{
	ifstream readFile;
	int size;
	string str;

	try
	{
		readFile.open(WORLDMAP_DATA_PATH);
		if (readFile.is_open())
		{
			readFile >> str;
			size = stoi(str);

			for (int i = 0; i < 2; i++)
			{
				readFile >> str;    // mapPath, eventPath 구분할려고 문자열 넣은거 없애는 부분
				for (int t = 0; t < size; t++) // 배경, 오브젝트, 콜라이더 데이터를 받는다.
				{
					switch (i)
					{
					case 0:
						readFile >> str;	// 실제 데이터 받는 부분
						protalPath.emplace_back(str);
						break;
					case 1:
						readFile >> str;	// 실제 데이터 받는 부분
						portalEventPath.emplace_back(str);
						break;
					}
				}
			}
		}
	}
	catch (const std::exception&)
	{
	}

	readFile.close();
}

void WorldMapManager::LoadMapData(const GameState state, const int stage)
{
	ifstream readFile;
	int value[2];       // 처음 값 2개를 받을 변수
	WorldMap mapData;   // 불러온 맵의 값 저장할 변수
	MapEdittorSelectState selectState;
	string str;

	const int DATA_KIND = 3;

	try
	{
		readFile.open(protalPath[stage]);
		if (readFile.is_open())
		{

			for (int i = 0; i < 2; i++) // 맵의 크기, x,y값을 받고
			{
				readFile >> str;
				value[i] = stoi(str);
			}

			for (int i = 0; i < DATA_KIND; i++) // 배경, 오브젝트, 콜라이더 데이터를 받는다.
			{
				readFile >> str;    // background, objects, colider 구분할려고 문자열 넣은거 없애는 부분

				switch (i)
				{
				case 0:
					selectState = MapEdittorSelectState::BACKGROUND;     break;
				case 1:
					selectState = MapEdittorSelectState::OBJECT;         break;
				case 2:
					selectState = MapEdittorSelectState::COLLIDER;      break;
				}

				for (int y = 0; y < value[0]; y++)
				{
					for (int x = 0; x < value[1]; x++)
					{
						readFile >> str;
						mapData.SetData(selectState, { x,y }, stoi(str));
					}
				}
			}
		}

		switch (state)
		{
		case GameState::MAPEDITTOR:
			MapEdittor::GetInstance()->SetWorldMapData(mapData);
			break;
		case GameState::INGAME:
			SetWorldMap(mapData);
			break;
		default:
			break;
		}

	}
	catch (const std::exception&)
	{
	}

	readFile.close();
}

void WorldMapManager::LoadMapData(const GameState state, const char* filePath)
{
	switch (state)
	{
	case GameState::MAPEDITTOR:
		break;
	case GameState::INGAME:
		break;
	default:
		return;

	}
	ifstream readFile;
	int value[2];       // 처음 값 2개를 받을 변수
	WorldMap mapData;   // 불러온 맵의 값 저장할 변수
	MapEdittorSelectState selectState;
	string str;

	const int DATA_KIND = 3;

	try
	{
		readFile.open(filePath);
		if (readFile.is_open())
		{

			for (int i = 0; i < 2; i++) // 맵의 크기, x,y값을 받고
			{
				readFile >> str;
				value[i] = stoi(str);
			}

			for (int i = 0; i < DATA_KIND; i++) // 배경, 오브젝트, 콜라이더 데이터를 받는다.
			{
				readFile >> str;    // background, objects, colider 구분할려고 문자열 넣은거 없애는 부분

				switch (i)
				{
				case 0:
					selectState = MapEdittorSelectState::BACKGROUND;     break;
				case 1:
					selectState = MapEdittorSelectState::OBJECT;         break;
				case 2:
					selectState = MapEdittorSelectState::COLLIDER;      break;
				}

				for (int y = 0; y < value[0]; y++)
				{
					for (int x = 0; x < value[1]; x++)
					{
						readFile >> str;
						mapData.SetData(selectState, { x,y }, stoi(str));
					}
				}
			}
		}
		
		switch (state)
		{
		case GameState::MAPEDITTOR:
			MapEdittor::GetInstance()->SetWorldMapData(mapData);
			break;
		case GameState::INGAME:
			SetWorldMap(mapData);
			break;
		default:
			break;
		}

	}
	catch (const std::exception&)
	{
	}

	readFile.close();
}

void WorldMapManager::LoadEventData(const int stage)
{
	ifstream readFile;
	int value[2];       // 처음 값 2개를 받을 변수
	string str;

	try
	{
		readFile.open(portalEventPath[stage]);
		if (readFile.is_open())
		{

			for (int i = 0; i < 2; i++) // 맵의 크기, x,y값을 받고
			{
				readFile >> str;
				value[i] = stoi(str);
			}

			readFile >> str;    // background, objects, colider 구분 문자열 제거

			for (int y = 0; y < value[0]; y++)
			{
				for (int x = 0; x < value[1]; x++)
				{
					readFile >> str;
					worldMap.SetData(MapEdittorSelectState::EVENT, { x,y }, stoi(str));
				}
			}
			
			readFile >> str;	// protalPosition 구분 문자열 제거
			// 포탈 세팅 코드
			protalData.clear();
			
			while (!readFile.eof())
			{
				readFile >> str;
				value[0] = stoi(str);	// x 좌표
				readFile >> str;
				value[1] = stoi(str);	// y 좌표
				readFile >> str;

				Portal portal;
				portal.pos = { value[1], value[0] };
				portal.stage = stoi(str);

				readFile >> str;
				value[0] = stoi(str);	// spawn x 좌표
				readFile >> str;
				value[1] = stoi(str);	// spawn y 좌표
				portal.spawnPos = { value[1], value[0] };

				protalData.emplace_back(portal);
			}
		}
	}
	catch (const std::exception&)
	{
	}

	readFile.close();
}
