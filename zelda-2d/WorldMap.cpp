#include "pch.h"
#include "WorldMap.h"

void WorldMap::Init()
{
	npcData.clear();
	fieldMonster.clear();
}

int WorldMap::GetData(SelectMapState state, POINT pos)
{
	if (!(0 <= pos.x && MAP_MAX_X > pos.x))
		return 0;
	if (!(0 <= pos.y && MAP_MAX_Y > pos.y))
		return 0;

	switch (state)
	{
	case SelectMapState::BACKGROUND:
		return backgroundData[pos.y][pos.x];
	case SelectMapState::OBJECT:
		return objectData[pos.y][pos.x];
	case SelectMapState::COLLIDER:
		return coliderData[pos.y][pos.x];
	case SelectMapState::EVENT:
		return eventData[pos.y][pos.x];
	default:
		return 0;
	}
}

void WorldMap::SetData(SelectMapState state, POINT pos, int data)
{
	if (!(0 <= pos.x && MAP_MAX_X > pos.x))
		return ;
	if (!(0 <= pos.y && MAP_MAX_Y > pos.y))
		return ;

	switch (state)
	{
	case SelectMapState::BACKGROUND:
		backgroundData[pos.y][pos.x] = data;
		break;
	case SelectMapState::OBJECT:
		objectData[pos.y][pos.x] = data;
		break;
	case SelectMapState::COLLIDER:
		if(0 != data)
			coliderData[pos.y][pos.x] = true;
		else
			coliderData[pos.y][pos.x] = false;
		break;
	case SelectMapState::EVENT:
		eventData[pos.y][pos.x] = data;
		break;
	default:
		return;
	}
}

vector<FieldNPC>* WorldMap::GetNPCData()
{
	return &npcData;
}

void WorldMap::SetNPCData(const FieldNPC fieldNPC)
{
	npcData.emplace_back(fieldNPC);
}

vector<Monster>* WorldMap::GetMonsterData()
{
	return &fieldMonster;
}

void WorldMap::SetMonsterData(const Monster monster)
{
	fieldMonster.emplace_back(monster);
}
