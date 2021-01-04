#include "pch.h"
#include "WorldMap.h"

int WorldMap::GetData(MapEdittorSelectState state, POINT pos)
{
	if (!(0 <= pos.x && MAP_MAX_X > pos.x))
		return 0;
	if (!(0 <= pos.y && MAP_MAX_Y > pos.y))
		return 0;

	switch (state)
	{
	case MapEdittorSelectState::BACKGROUND:
		return backgroundData[pos.y][pos.x];
	case MapEdittorSelectState::OBJECT:
		return objectData[pos.y][pos.x];
	case MapEdittorSelectState::COLIDER:
		return coliderData[pos.y][pos.x];
	default:
		return 0;
	}
}

void WorldMap::SetData(MapEdittorSelectState state, POINT pos, int data)
{
	if (!(0 <= pos.x && MAP_MAX_X > pos.x))
		return ;
	if (!(0 <= pos.y && MAP_MAX_Y > pos.y))
		return ;

	switch (state)
	{
	case MapEdittorSelectState::BACKGROUND:
		backgroundData[pos.y][pos.x] = data;
		break;
	case MapEdittorSelectState::OBJECT:
		objectData[pos.y][pos.x] = data;
		break;
	case MapEdittorSelectState::COLIDER:
		coliderData[pos.y][pos.x] = data;
		break;
	default:
		return;
	}
}
