#include "pch.h"
#include "WorldMapManager.h"
#include "InteractionManager.h"
#include "MapEdittor.h"
#include <fstream>
#include <string>

constexpr const POINT RETOUCH_WOOD_DOOR_POS{ 2,5 };
constexpr const POINT SPAWN_PLAYER_STAGE1_POS{ 12,13 };

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
	WorldMap* worldMap = WorldMapManager::GetInstance()->GetWorldMap();

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
	WorldMap * worldMap = WorldMapManager::GetInstance()->GetWorldMap();

	switch (worldMap->GetData(MapEdittorSelectState::EVENT, pos))
	{
	case Event::OPEN_WOOD_HOUSE_DOOR:			// 오두막 문 열기,
		OpenWoodHouseDoor(pos);
		break;
	case Event::CLOSE_WOOD_HOUSE_DOOR:			// 오두막 문 닫기,
		CloseWoodHouseDoor(pos);
		break;
	////case Event::OPEN_BOX:	// 아이템 드랍
	////	gameManager->AddItem(gameManager->GetCurrentStage(),
	////		Item({pos.x*TILE_SIZE, pos.y*TILE_SIZE}, ItemInfo::MONEY, 5));	// 금액 수정, 아이템 랜덤 드랍으로 수정 필요함
	////	worldMap->SetData(MapEdittorSelectState::EVENT, { pos.x,pos.y}, Event::NONE);	// 기존 이벤트 삭제
	////	break;
	////case Event::MOVE_STAGE_1:
	////	if (MapInfo::WOOD_HOUSE == gameManager->GetCurrentStage())
	////		gameManager->GetPlayer()->SetPos({ 
	////		static_cast<double>(STAGE2_SPAWN_POS.x),static_cast<double>(STAGE2_SPAWN_POS.y)});
	////	gameManager->SetCurrentStage(MapInfo::NORMAL);
	////	break;
	////case Event::MOVE_STAGE_2:	
	////	gameManager->SetCurrentStage(MapInfo::WOOD_HOUSE);
	////	gameManager->GetPlayer()->SetPos
	////	({ static_cast<double>(STAGE2_SPAWN_POINT.x),static_cast<double>(STAGE2_SPAWN_POINT.y)});

	//	//LoadTextMapData(GameState::INGAME, STAGE2_PATH);     // 인게임에서 사용할 맵데이터 로드
	//	//LoadTextEventData(STAGE2_EVENT_PATH, gameManager->GetWorldMapData());
	//	break;
		default:
		break;
	}
}

void InteractionManager::OpenWoodHouseDoor(const POINT pos)
{
	for (int y = 0; y < MAP_MAX_Y; y++)
	{
		for (int x = 0; x < MAP_MAX_X; x++)
		{
			if (TextureName::wood_house_close == WorldMapManager::GetInstance()->GetWorldMap()->GetData(MapEdittorSelectState::OBJECT, { x,y }))
			{
				WorldMapManager::GetInstance()->GetWorldMap()->SetData(MapEdittorSelectState::OBJECT, { x,y }, TextureName::wood_house);		// 오두막 문 열기
				WorldMapManager::GetInstance()->GetWorldMap()->SetData(MapEdittorSelectState::EVENT, pos, Event::CLOSE_WOOD_HOUSE_DOOR);		// 오두막 문닫히는 이벤트 등록
						
				Portal protal;
				protal.pos = { x+ RETOUCH_WOOD_DOOR_POS.x,y + RETOUCH_WOOD_DOOR_POS.y};
				protal.stage = 1;
				protal.spawnPos = { SPAWN_PLAYER_STAGE1_POS.x,SPAWN_PLAYER_STAGE1_POS.y };
				WorldMapManager::GetInstance()->AddProtalData(protal);
				return;
			}
		}
	}
}

void InteractionManager::CloseWoodHouseDoor(const POINT pos)
{
	for (int y = 0; y < MAP_MAX_Y; y++)
	{
		for (int x = 0; x < MAP_MAX_X; x++)
		{
			if (TextureName::wood_house == WorldMapManager::GetInstance()->GetWorldMap()->GetData(MapEdittorSelectState::OBJECT, { x,y }))
			{
				WorldMapManager::GetInstance()->GetWorldMap()->SetData(MapEdittorSelectState::OBJECT, { x,y }, TextureName::wood_house_close);	// 오두막 문 닫기
				WorldMapManager::GetInstance()->GetWorldMap()->SetData(MapEdittorSelectState::EVENT, pos, Event::OPEN_WOOD_HOUSE_DOOR);	// 오두막 문열리는 이벤트 등록

				int count = 0;
				for (const auto& iterator : WorldMapManager::GetInstance()->GetProtalData())
				{
					if (iterator.stage == 1 &&
						iterator.pos.x == x + RETOUCH_WOOD_DOOR_POS.x &&
						iterator.pos.y == y + RETOUCH_WOOD_DOOR_POS.y)
					{
						WorldMapManager::GetInstance()->DeleteProtalData(count);
						return;
					}
					count++;
				} 

				return;
			}
		}
	}
}
