#include "pch.h"
#include "GameManager.h"

void GameManager::Run()
{

}

GameManager::GameManager()
{
	state = GameState::MAIN;
}

const GameState GameManager::GetState()
{
	return state;
}

void GameManager::SetState(const GameState state)
{
	this->state = state;
}
