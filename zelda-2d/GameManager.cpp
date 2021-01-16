#include "pch.h"
#include "GameManager.h"

GameManager* GameManager::instance = nullptr;

GameManager::GameManager()
{
	state = GameState::MAIN;
	player = nullptr;
	npc = nullptr;
	time = Time::GetInstance();
	interactionManager = InteractionManager::GetInstance();
}

GameManager::~GameManager()
{
	if(player != nullptr)
		delete player;
	player = nullptr;
	if (npc != nullptr)
		delete npc;
	npc = nullptr;

	time->ReleaseInstance();
	time = nullptr;
	interactionManager->ReleaseInstance();
	interactionManager = nullptr;
}

GameManager* GameManager::GetInstance()
{
	if (nullptr == instance)
		instance = new GameManager();

	return instance;
}

void GameManager::ReleaseInstance()
{
	delete instance;
	instance = nullptr;
}

void GameManager::Run()
{
	DPOINT prevPos = player->GetPos();	
	player->Input(time->Update());
	switch (player->GetState())
	{
	case CharacterInfo::WALK:
		LimitMoveMent(prevPos);					// 맵 외곽 및 콜라이더 위치 이동 제한
		break;
	case CharacterInfo::INTERACTION:
		POINT pivotPos = { static_cast<LONG>(player->GetPos().x),  static_cast<LONG>(player->GetPos().y) };
		pivotPos.x += PLAYER_PIVOT_POS.x;
		pivotPos.y += PLAYER_PIVOT_POS.y;

		pivotPos = worldMap.ChangePosToMapPoint(pivotPos);	// 맵 좌표로 변환
		interactionManager->ChangeMapData(&worldMap ,pivotPos, player->GetDir());
		break;
	}
}

void GameManager::LimitMoveMent(const DPOINT prevDPos)
{
	POINT pos = { static_cast<LONG>(player->GetPos().x),  static_cast<LONG>(player->GetPos().y) };
	if (0 > pos.x || ClientSize::width- LIMIT_MAP_X_CORRECTION < pos.x)
	{
		player->SetPos(prevDPos);
		return;
	}
	if (0 > pos.y || ClientSize::height- LIMIT_MAP_Y_CORRECTION < pos.y)
	{
		player->SetPos(prevDPos);
		return;
	}

	POINT colliderPos[4] = { {pos.x + PLAYER_COLLIDER_BOX_LEFT_TOP.x, pos.y + PLAYER_COLLIDER_BOX_LEFT_TOP.y},
		{pos.x + PLAYER_COLLIDER_BOX_LEFT_TOP.x, pos.y + PLAYER_COLLIDER_BOX_RIGHT_BOTTOM.y},
		{ pos.x + PLAYER_COLLIDER_BOX_RIGHT_BOTTOM.x, pos.y + PLAYER_COLLIDER_BOX_LEFT_TOP.y},
		 {pos.x + PLAYER_COLLIDER_BOX_RIGHT_BOTTOM.x, pos.y + PLAYER_COLLIDER_BOX_RIGHT_BOTTOM.y} };

	for (int i = 0; i < sizeof(colliderPos)/sizeof(colliderPos[0]); i++)
	{
		colliderPos[i] = worldMap.ChangePosToMapPoint(colliderPos[i]);	// 맵상의 좌표로 변환 후

		if (0 != worldMap.GetData(MapEdittorSelectState::COLLIDER, colliderPos[i]))	// 콜라이더 위에 위치하고 있는 경우
		{
			player->SetPos(prevDPos);
			return;
		}
	}
}

const GameState GameManager::GetState()
{
	return state;
}

void GameManager::SetState(const GameState state)
{
	this->state = state;
}

void GameManager::SetWorldMapData(const WorldMap worldMap)
{
	this->worldMap = worldMap;
}

const WorldMap GameManager::GetWorldMapData()
{
	return worldMap;
}

void GameManager::SetPlayer(Player* player)
{
	this->player = player;
}

Player* GameManager::GetPlayer()
{
	return player;
}

void GameManager::SetNPC(NPC* npc)
{
	this->npc = npc;
}

NPC* GameManager::GetNPC()
{
	return npc;
}
