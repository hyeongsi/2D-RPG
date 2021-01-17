#include "pch.h"
#include "GameManager.h"
#include "InteractionManager.h"
#include "MapEdittor.h"
#include <fstream>
#include <string>

InteractionManager* InteractionManager::instance = nullptr;

InteractionManager::InteractionManager()
{

}

InteractionManager::~InteractionManager()
{

}

InteractionManager* InteractionManager::GetInstance()
{
	if (nullptr == instance)
		instance = new InteractionManager();

	return instance;
}

void InteractionManager::ReleaseInstance()
{
	delete instance;
	instance = nullptr;
}

void InteractionManager::ChangeMapData(POINT pos)
{
	WorldMap* worldMap = GameManager::GetInstance()->GetWorldMapData(GameManager::GetInstance()->GetCurrentStage());

	// 맵 기준 좌표 이탈 시 무시
	if (!(0 <= pos.x && MAP_MAX_X > pos.x))
		return;
	if (!(0 <= pos.y && MAP_MAX_Y > pos.y))
		return;

	switch (worldMap->GetData(MapEdittorSelectState::OBJECT, pos))
	{
	case TextureName::lever_off:		// 레버 on 상태로 변경
		worldMap->SetData(MapEdittorSelectState::OBJECT, pos, TextureName::lever_on);
		break;
	case TextureName::lever_on:			// 레버 off 상태로 변경
		worldMap->SetData(MapEdittorSelectState::OBJECT, pos, TextureName::lever_off);
		break;
	case TextureName::box_off:			// 상자 on 상태로 변경
		worldMap->SetData(MapEdittorSelectState::OBJECT, pos, TextureName::box_on);
		break;
	default:
		return;
	}
}

void InteractionManager::ActionEvent(const POINT pos)
{
	GameManager* gameManager = GameManager::GetInstance();
	WorldMap * worldMap = gameManager->GetWorldMapData(gameManager->GetCurrentStage());

	switch (worldMap->GetData(MapEdittorSelectState::EVENT, pos))
	{
	case Event::OPEN_WOOD_HOUSE_DOOR:			// 오두막 문 열기,
		for (int y = 0; y < MAP_MAX_Y; y++)
		{
			for (int x = 0; x < MAP_MAX_X; x++)
			{
				if (TextureName::wood_house_close == worldMap->GetData(MapEdittorSelectState::OBJECT, { x,y }))
				{
					worldMap->SetData(MapEdittorSelectState::OBJECT, { x,y }, TextureName::wood_house);		// 오두막 문 열기
					worldMap->SetData(MapEdittorSelectState::EVENT, pos, Event::CLOSE_WOOD_HOUSE_DOOR);		// 오두막 문닫히는 이벤트 등록
					
					worldMap->SetData(MapEdittorSelectState::EVENT, {x + WOOD_HOUSE_DOOR_POS.x ,y + WOOD_HOUSE_DOOR_POS .y}, Event::MOVE_STAGE_2);	// 오두막집 열리면 입구쪽 출입 이벤트 등록
					break;
				}
			}
		}
		break;
	case Event::CLOSE_WOOD_HOUSE_DOOR:			// 오두막 문 닫기,
		for (int y = 0; y < MAP_MAX_Y; y++)
		{
			for (int x = 0; x < MAP_MAX_X; x++)
			{
				if (TextureName::wood_house == worldMap->GetData(MapEdittorSelectState::OBJECT, { x,y }))
				{
					worldMap->SetData(MapEdittorSelectState::OBJECT, { x,y }, TextureName::wood_house_close);	// 오두막 문 닫기
					worldMap->SetData(MapEdittorSelectState::EVENT, pos, Event::OPEN_WOOD_HOUSE_DOOR);	// 오두막 문열리는 이벤트 등록
					
					worldMap->SetData(MapEdittorSelectState::EVENT, { x + WOOD_HOUSE_DOOR_POS.x,y + WOOD_HOUSE_DOOR_POS .y}, Event::NONE);	// 오두막집 출입 이벤트 삭제
					break;
				}
			}
		}
		break;
	case Event::OPEN_BOX:
		// 아이템 드랍
		break;
	case Event::MOVE_STAGE_1:
		if (MapInfo::WOOD_HOUSE == gameManager->GetCurrentStage())
			gameManager->GetPlayer()->SetPos({ 642.0,115.0 });	// 추후에 수정
		gameManager->SetCurrentStage(MapInfo::NORMAL);
		break;
	case Event::MOVE_STAGE_2:	
		gameManager->SetCurrentStage(MapInfo::WOOD_HOUSE);
		gameManager->GetPlayer()->SetPos
		({ static_cast<double>(STAGE2_SPAWN_POINT.x),static_cast<double>(STAGE2_SPAWN_POINT.y)});

		if (!(gameManager->GetWorldMapData(gameManager->GetCurrentStage())->GetIsLoad()))	// 로드 했던거면 다시 로드 X
		{
			LoadTextMapData(GameState::INGAME, STAGE2_PATH);     // 인게임에서 사용할 맵데이터 로드
			LoadTextEventData(STAGE2_EVENT_PATH, gameManager->GetWorldMapData(gameManager->GetCurrentStage()));
		}
		break;
	default:
		break;
	}

}

void InteractionManager::LoadTextMapData(const GameState state, const char* filePath)
{
	ifstream readFile;
	int value[2];       // 처음 값 2개를 받을 변수
	WorldMap mapData;   // 불러온 맵의 값 저장할 변수
	MapEdittorSelectState selectState;
	string str;

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

			for (int i = 0; i < 3; i++) // 배경, 오브젝트, 콜라이더 데이터를 받는다.
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

		mapData.SetIsLoad();	// 로드 여부 체크

		switch (state)
		{
		case GameState::MAPEDITTOR:
			MapEdittor::GetInstance()->SetWorldMapData(mapData);
			break;
		case GameState::INGAME:
			GameManager::GetInstance()->SetWorldMapData(mapData, GameManager::GetInstance()->GetCurrentStage());
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

void InteractionManager::LoadTextEventData(const char* filePath, WorldMap* worldMap)
{
	ifstream readFile;
	int value[2];       // 처음 값 2개를 받을 변수
	string str;

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

			readFile >> str;    // background, objects, colider 구분할려고 문자열 넣은거 없애는 부분

			for (int y = 0; y < value[0]; y++)
			{
				for (int x = 0; x < value[1]; x++)
				{
					readFile >> str;
					worldMap->SetData(MapEdittorSelectState::EVENT, { x,y }, stoi(str));
				}
			}

		}
	}
	catch (const std::exception&)
	{
	}

	readFile.close();
}
