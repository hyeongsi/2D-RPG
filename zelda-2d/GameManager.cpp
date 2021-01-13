#include "pch.h"
#include "GameManager.h"

GameManager* GameManager::instance = nullptr;

GameManager::GameManager()
{
	state = GameState::MAIN;
	character = nullptr;
	time = Time::GetInstance();
}

GameManager::~GameManager()
{
	delete character;
	character = nullptr;
	time->ReleaseInstance();
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
	DPOINT prevPos = character->GetPos();	
	character->Input(time->Update());
	LimitMoveMent(prevPos);					// 맵 외곽 및 콜라이더 위치 이동 제한

}

void GameManager::LimitMoveMent(const DPOINT prevDPos)
{
	POINT pos = { character->GetPos().x,  character->GetPos().y };
	if (0 > pos.x || ClientSize::width- LIMIT_MAP_X_CORRECTION < pos.x)
	{
		character->SetPos(prevDPos);
		return;
	}
	if (0 > pos.y || ClientSize::height- LIMIT_MAP_Y_CORRECTION < pos.y)
	{
		character->SetPos(prevDPos);
		return;
	}

	pos.x += CHAR_NORMAL_X;			// 기준 좌표값 ++
	pos.y += CHAR_NORMAL_Y;			// 기준 좌표값 ++

	pos = worldMap.ChangePosToMapPoint(pos);	// 맵상의 좌표로 변환 후

	if (0 != worldMap.GetData(MapEdittorSelectState::COLLIDER, pos))	// 콜라이더 위에 위치하고 있는 경우
	{
		character->SetPos(prevDPos);
		return;
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

void GameManager::SetCharacter(Character* character)
{
	this->character = character;
}

const Character GameManager::GetCharacter()
{
	return *character;
}
