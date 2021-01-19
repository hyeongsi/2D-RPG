#include "pch.h"
#include "GameManager.h"
#include "InteractionManager.h"

GameManager* GameManager::instance = nullptr;

GameManager::GameManager()
{
	tick = GetTickCount64();

	state = GameState::MAIN;
	currentStage = 0;
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

		pivotPos = worldMap[currentStage].ChangePosToMapPoint(pivotPos);	// 맵 좌표로 변환
		// 캐릭터가 보고있는 방향의 오브젝트의 상호작용 실행 시키기 위해 연산
		switch (player->GetDir())
		{
		case CharacterInfo::DOWN:
			pivotPos.y += 1;
			break;
		case CharacterInfo::RIGHT:
			pivotPos.x += 1;
			break;
		case CharacterInfo::UP:
			pivotPos.y -= 1;
			break;
		case CharacterInfo::LEFT:
			pivotPos.x -= 1;
			break;
		default:
			return;
		}

		// 이벤트에 딜레이 추가
		if (GetTickCount64() > tick + TICK_DELAY)
			tick = GetTickCount64();
		else
			break;

		interactionManager->ChangeMapData(pivotPos);		// 오브젝트 애니메이션 변경
		interactionManager->ActionEvent(pivotPos);			// 연결 이벤트 발생

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
		colliderPos[i] = worldMap[currentStage].ChangePosToMapPoint(colliderPos[i]);	// 맵상의 좌표로 변환 후

		if (0 != worldMap[currentStage].GetData(MapEdittorSelectState::COLLIDER, colliderPos[i]))	// 콜라이더 위에 위치하고 있는 경우
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

void GameManager::SetWorldMapData(const WorldMap worldMap, const int index)
{
	if (0 <= index && STAGE_SIZE > index)
		this->worldMap[index] = worldMap;
}

WorldMap* GameManager::GetWorldMapData(const int index)
{
	if(0 <= index && STAGE_SIZE > index)
		return &worldMap[index];
	else
		return &worldMap[0];
}

void GameManager::SetCurrentStage(const int currentStage)
{
	if(0 <= currentStage && STAGE_SIZE > currentStage)
		this->currentStage = currentStage;
}

const int GameManager::GetCurrentStage()
{
	return currentStage;
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

void GameManager::AddItem(const int stage, const Item item)
{
	if(0 <= stage && stage < STAGE_SIZE)
		(FieldItem[stage]).emplace_back(item);
}

void GameManager::DeleteItem(const int stage, const int index)
{
	if (0 <= index && index < static_cast<int>((FieldItem[stage]).size()))
	{
		int count = 0;
		for (auto iterator = (FieldItem[stage]).begin(); iterator != (FieldItem[stage]).end();)
		{
			if (index == count)
			{
				(FieldItem[stage]).erase(iterator);
				return;
			}

			count++;
			iterator++;
		}
	}
}

const Item GameManager::GetItem(const int stage, const int index)
{
	if (0 <= index && index < static_cast<int>((FieldItem[stage]).size()))
		return (FieldItem[stage])[index];
	else
		return Item();
}

vector<Item> GameManager::GetFieldItem(const int stage)
{
	return (FieldItem[stage]);
}
