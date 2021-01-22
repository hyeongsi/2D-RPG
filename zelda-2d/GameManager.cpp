#include "pch.h"
#include "GameManager.h"
#include "InteractionManager.h"
#include "resource.h"
#include "WorldMapManager.h"

GameManager* GameManager::instance = nullptr;

GameManager::GameManager()
{
	inputTick = GetTickCount64();
	eventTick = GetTickCount64();

	state = GameState::MAIN;
	player = nullptr;
	inventory = nullptr;
	npc = nullptr;
	time = Time::GetInstance();
	interactionManager = InteractionManager::GetInstance();
}

GameManager::~GameManager()
{
	if(player != nullptr)
		delete player;
	player = nullptr;
	if (inventory != nullptr)
		delete inventory;
	inventory = nullptr;
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

void GameManager::Input()
{
	if (GetAsyncKeyState(0x49) & 0x8000)
	{
		if (GetTickCount64() > inputTick + INPUT_DELAY)
			inputTick = GetTickCount64();
		else
			return;


		if (inventory->IsOpen())
		{
			inventory->SetOpen(false);
			SendMessage(g_hWnd, WM_COMMAND, IDC_lNVEN_CLOSE, 0);    // 버튼 선택상태 초기화
		}
		else
		{
			inventory->SetOpen(true);
			SendMessage(g_hWnd, WM_COMMAND, IDC_lNVEN_OPEN, 0);    // 버튼 선택상태 초기화
		}
	}
}

void GameManager::Run()
{
	DPOINT prevPos = player->GetPos();	
	player->Input(time->Update());
	POINT pivotPos;

	switch (player->GetState())
	{
	case CharacterInfo::WALK:
		LimitMoveMent(prevPos);						// 맵 외곽 및 콜라이더 위치 이동 제한
		UsePortal();								// 포탈 이동 관련 코드
		
		break;
	case CharacterInfo::INTERACTION:	
		if (GetTickCount64() > eventTick + EVENT_DELAY)			// 이벤트에 딜레이 추가
			eventTick = GetTickCount64();
		else
			break;

		pivotPos = GetPlayerPivotMapPoint();
		
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

		interactionManager->ChangeMapData(pivotPos);		// 오브젝트 애니메이션 변경
		interactionManager->ActionEvent(pivotPos);			// 연결 이벤트 발생

		break;
	}
}

void GameManager::test()
{
	for (int y = 0; y < MAP_MAX_Y; y++)
	{
		for (int x = 0; x < MAP_MAX_X; x++)
		{
			if (TextureName::wood_house_close == WorldMapManager::GetInstance()->GetWorldMap()->GetData(MapEdittorSelectState::OBJECT, { x,y }))
			{
				WorldMapManager::GetInstance()->GetWorldMap()->SetData(MapEdittorSelectState::OBJECT, { x,y }, TextureName::wood_house);		// 오두막 문 열기	
				WorldMapManager::GetInstance()->GetWorldMap()->SetData(MapEdittorSelectState::EVENT, {x + WOOD_HOUSE_DOOR_POS.x ,y + WOOD_HOUSE_DOOR_POS .y}, Event::MOVE_STAGE_2);	// 오두막집 열리면 입구쪽 출입 이벤트 등록
				break;
			}
		}
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
		colliderPos[i] = WorldMapManager::GetInstance()->ChangePosToMapPoint(colliderPos[i]);	// 맵상의 좌표로 변환 후

		if (0 != WorldMapManager::GetInstance()->GetWorldMap()->GetData(MapEdittorSelectState::COLLIDER, colliderPos[i]))	// 콜라이더 위에 위치하고 있는 경우
		{
			player->SetPos(prevDPos);
			return;
		}
	}
}

void GameManager::UsePortal()
{
	POINT pivotPos = GetPlayerPivotMapPoint();		// 피벗 좌표를 기준으로 캐릭터의 맵 좌표 가져옴
	for (auto& iterator : WorldMapManager::GetInstance()->GetProtalData())
	{
		if (iterator.pos.x == pivotPos.x && iterator.pos.y == pivotPos.y)
		{
			WorldMapManager::GetInstance()->SetCurrentStage(iterator.stage);
			WorldMapManager::GetInstance()->LoadMapData(GameState::INGAME, iterator.stage);
			WorldMapManager::GetInstance()->LoadEventData(iterator.stage);

			player->SetPos({ static_cast<double>(iterator.spawnPos.x) * TILE_SIZE, 
				static_cast<double>(iterator.spawnPos.y) * TILE_SIZE });
			break;
		}
	}
}

POINT GameManager::GetPlayerPivotMapPoint()
{
	POINT pivotPos = { static_cast<LONG>(player->GetPos().x),  static_cast<LONG>(player->GetPos().y) };
	pivotPos.x += PLAYER_PIVOT_POS.x;
	pivotPos.y += PLAYER_PIVOT_POS.y;

	pivotPos = WorldMapManager::GetInstance()->ChangePosToMapPoint(pivotPos);	// 맵 좌표로 변환

	return pivotPos;
}

const GameState GameManager::GetState()
{
	return state;
}

void GameManager::SetState(const GameState state)
{
	this->state = state;
}

void GameManager::SetPlayer(Player* player)
{
	this->player = player;
}

Player* GameManager::GetPlayer()
{
	return player;
}

void GameManager::SetInventory(Inventory* inventory)
{
	this->inventory = inventory;
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
