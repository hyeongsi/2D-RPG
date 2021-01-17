#include "pch.h"
#include "InteractionManager.h"

InteractionManager* InteractionManager::instance = nullptr;

InteractionManager::InteractionManager()
{
	tick = GetTickCount64();
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

void InteractionManager::ChangeMapData(WorldMap* worldMap, POINT pos, const int dir)
{
	if (GetTickCount64() > tick + tickDelay)
	{
		tick = GetTickCount64();
	}
	else
		return;

	switch (dir)
	{
	case CharacterInfo::DOWN:
		pos.y += 1;
		break;
	case CharacterInfo::RIGHT:
		pos.x += 1;
		break;
	case CharacterInfo::UP:
		pos.y -= 1;
		break;
	case CharacterInfo::LEFT:
		pos.x -= 1;
		break;
	default:
		return;
	}

	// 맵 기준 좌표 이탈 시 무시
	if (!(0 <= pos.x && MAP_MAX_X > pos.x))
		return;
	if (!(0 <= pos.y && MAP_MAX_Y > pos.y))
		return;

	switch (worldMap->GetData(MapEdittorSelectState::OBJECT, pos))
	{
	case TextureName::lever_off:
		worldMap->SetData(MapEdittorSelectState::OBJECT, pos, TextureName::lever_on);
		break;
	case TextureName::lever_on:
		worldMap->SetData(MapEdittorSelectState::OBJECT, pos, TextureName::lever_off);
		break;
	case TextureName::box_off:
		worldMap->SetData(MapEdittorSelectState::OBJECT, pos, TextureName::box_on);
		break;
	default:
		return;
	}

	ActionEvent(worldMap, pos);		// 매칭된 이벤트 실행
}

void InteractionManager::ActionEvent(WorldMap* worldMap, const POINT pos)
{
	switch (worldMap->GetData(MapEdittorSelectState::EVENT, pos))
	{
	case Event::OPEN_WOOD_HOUSE_DOOR:
		for (int y = 0; y < MAP_MAX_Y; y++)
		{
			for (int x = 0; x < MAP_MAX_X; x++)
			{
				if (TextureName::wood_house_close == worldMap->GetData(MapEdittorSelectState::OBJECT, { x,y }))
				{
					worldMap->SetData(MapEdittorSelectState::OBJECT, { x,y }, TextureName::wood_house);
					y = MAP_MAX_Y;
				}
			}
		}
		break;
	case Event::OPEN_BOX:
		// 아이템 드랍
		break;
	default:
		break;
	}

}
