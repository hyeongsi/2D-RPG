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
	// 이동 제한 코드 작성
	character->Input(time->Update());


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
