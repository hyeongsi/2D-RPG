#include "pch.h"
#include "GameManager.h"
#include "InteractionManager.h"
#include "resource.h"
#include "WorldMapManager.h"
#include "ItemManager.h"
#include "NPCManager.h"
#include "ImageManager.h"

GameManager* GameManager::instance = nullptr;

GameManager::GameManager()
{
	inputTick = GetTickCount64();
	eventTick = GetTickCount64();
	attackTick = GetTickCount64();

	state = GameState::MAIN;
	player = nullptr;
	inventory = nullptr;
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
			inventory->SetOpen(false);
		else
			inventory->SetOpen(true);
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
		NPCManager::GetInstance()->InitInteractNPCInfo();	// 상점 이용 중이라면 취소
		LimitMoveMent(prevPos);						// 맵 외곽 및 콜라이더 위치 이동 제한
		PickUpItem();								// 아이템 파밍
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
	case CharacterInfo::ATTACK:
		AttackMonster();	// 몬스터 피격 처리
		DieMonster();		// 몬스터 죽음 처리

		break;
	}
}

void GameManager::LimitMoveMent(const DPOINT prevDPos)
{
	constexpr const int LIMIT_MAP_X_CORRECTION = 32;	// 맵 밖으로 나가는 경우 보정 크기
	constexpr const int LIMIT_MAP_Y_CORRECTION = 46;	// 맵 밖으로 나가는 경우 보정 크기

	constexpr const POINT PLAYER_COLLIDER_BOX_LEFT_TOP = { 10,47 };	// 캐릭터 콜라이더 기준 좌표 
	constexpr const POINT PLAYER_COLLIDER_BOX_RIGHT_BOTTOM = { 23,51 };	// 캐릭터 콜라이더 기준 좌표

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

	POINT mapColliderPos[4];

	for (int i = 0; i < sizeof(colliderPos)/sizeof(colliderPos[0]); i++)
	{
		mapColliderPos[i] = WorldMapManager::GetInstance()->ChangePosToMapPoint(colliderPos[i]);	// 맵상의 좌표로 변환 후

		if (0 != WorldMapManager::GetInstance()->GetWorldMap()->GetData(SelectMapState::COLLIDER, mapColliderPos[i]))	// 콜라이더 위에 위치하고 있는 경우
		{
			player->SetPos(prevDPos);

			RetouchMoveMent(colliderPos);	// 위치 보정
			return;
		}
	}
}

void GameManager::PickUpItem()
{
	POINT pivotPos = GetPlayerPivotMapPoint();		// 피벗 좌표를 기준으로 캐릭터의 맵 좌표 가져옴

	for (auto iterator = (*ItemManager::GetInstance()->GetFieldItem()).begin(); iterator != (*ItemManager::GetInstance()->GetFieldItem()).end();)
	{
		if ((*iterator).pos.x == pivotPos.x && (*iterator).pos.y == pivotPos.y)				// 아이템과 위치가 같으면
		{
			if (inventory->GetLastItemIndex() == INVEN_SIZE)		// 인벤토리 풀이면 아이템 파밍 X
				return;

			inventory->SetItem((*ItemManager::GetInstance()->GetItemData())[(*iterator).index-1]);	// (아이템 매니저의 정보를 가지고) 인벤토리에 아이템 추가 
			(*ItemManager::GetInstance()->GetFieldItem()).erase(iterator);			// 필드 아이템은 삭제 처리
			return;
		}
		else
			iterator++;
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

			ItemManager::GetInstance()->GetFieldItem()->clear();	// 이동 시 필드 아이템 삭제
			break;
		}
	}
}

void GameManager::RetouchMoveMent(POINT colliderPos[4])
{
	int posIndex[2] = {1,3};	// side1 (left, up), side2 (right, down)
	bool moveX = false;	// dir (false : y,  true : x)
	switch (player->GetDir())	// 방향에 따라 양쪽 콜라이더 위치로 보정
	{
	case CharacterInfo::DOWN:
		posIndex[0] = 1;
		posIndex[1] = 3;
		moveX = false;	// y 값을 의미
		for (int i = 0; i < 2; i++)
			colliderPos[posIndex[i]].y += 1;
		break;
	case CharacterInfo::UP:
		posIndex[0] = 0;
		posIndex[1] = 2;
		moveX = false;	// y 값을 의미
		for (int i = 0; i < 2; i++)
			colliderPos[posIndex[i]].y -= 1;
		break;
	case CharacterInfo::LEFT:
		posIndex[0] = 0;
		posIndex[1] = 1;
		moveX = true;	// x 값을 의미
		for (int i = 0; i < 2; i++)
			colliderPos[posIndex[i]].x -= 1;
		break;
	case CharacterInfo::RIGHT:
		posIndex[0] = 2;
		posIndex[1] = 3;
		moveX = true;	// x 값을 의미
		for (int i = 0; i < 2; i++)
			colliderPos[posIndex[i]].x += 1;
		break;
	}

	int fixValue = 3;
	for (int i = 0; i < 2; i++)
	{
		if (!moveX)	// y값 비교, up, down 일 경우// x값 비교, right, left 일 경우
			colliderPos[posIndex[i]].x -= fixValue;
		else		// x값 비교, left, right 일 경우
			colliderPos[posIndex[i]].y -= fixValue;

		if (0 == WorldMapManager::GetInstance()->GetWorldMap()->GetData(SelectMapState::COLLIDER, WorldMapManager::GetInstance()->ChangePosToMapPoint(colliderPos[posIndex[i]])))	// 콜라이더를 벗어난 경우
		{
			// 왼 -> 오,  위 -> 아래
			if (moveX && i == 0)	// 양옆방향, y+ 방향이 콜라이더를 벗어나는 것과 가까울 경우									
				player->SetPos({ player->GetPos().x, player->GetPos().y - 1 });
			else if(moveX && i == 1)	// 양옆방향, y- 방향이 콜라이더를 벗어나는 것과 가까울 경우	
				player->SetPos({ player->GetPos().x, player->GetPos().y + 1 });
			else if (!moveX && i == 0)	// 위아래방향, x- 방향이 콜라이더를 벗어나는 것과 가까울 경우		y방향, down 일경우, 왼쪽이 
				player->SetPos({ player->GetPos().x - 1, player->GetPos().y });
			else if (!moveX && i == 1)	// 위아래방향, x+ 방향이 콜라이더를 벗어나는 것과 가까울 경우		
				player->SetPos({ player->GetPos().x + 1, player->GetPos().y });
		}

		fixValue *= -1;
	}
}

void GameManager::AttackMonster()
{
	AnimationObject* attackAnimationObject = ImageManager::GetInstance()->GetPlayerAnimationData(TextureName::PLAYER_ATTACK);
	const int hitDamageDelay = 300;

	// 마지막 전 애니메이션 에서만 공격으로 인정
	if (attackAnimationObject->GetSelectBitmapIndex() != 
		(attackAnimationObject->GetBitmapCount(attackAnimationObject->GetSelectAnimationBitmapIndex()) - 1))
	{
		return;
	}

	if (GetTickCount64() > attackTick + hitDamageDelay)
	{
		attackTick = GetTickCount64();
	}
	else
		return;

	const int attackColliderSize = 26;	// 공격 범위
	const int pushedOutSize = 30;		// 피격 시 밀려나는 크기
	RECT colliderRect;
	DPOINT pushOutPos;
	DPOINT pushCorrectionPos;	// 콜라이더 접근 시 이동 보정 수치
	switch (player->GetDir())
	{
	case CharacterInfo::DOWN:
		colliderRect.left = static_cast<LONG>(player->GetPos().x + PLAYER_PIVOT_POS.x - attackColliderSize);
		colliderRect.top = static_cast<LONG>(player->GetPos().y + PLAYER_PIVOT_POS.y);
		colliderRect.right = static_cast<LONG>(player->GetPos().x + PLAYER_PIVOT_POS.x + attackColliderSize);
		colliderRect.bottom = static_cast<LONG>(player->GetPos().y + PLAYER_PIVOT_POS.y + (static_cast<double>(attackColliderSize)*2));

		pushOutPos = { 0,pushedOutSize };	// 밀려날 크기 설정
		pushCorrectionPos = { 0 , -1 };
		break;
	case CharacterInfo::RIGHT:
		colliderRect.left = static_cast<LONG>(player->GetPos().x + PLAYER_PIVOT_POS.x);
		colliderRect.top = static_cast<LONG>(player->GetPos().y + PLAYER_PIVOT_POS.y - attackColliderSize);
		colliderRect.right = static_cast<LONG>(player->GetPos().x + PLAYER_PIVOT_POS.x + (static_cast<double>(attackColliderSize) * 2));
		colliderRect.bottom = static_cast<LONG>(player->GetPos().y + PLAYER_PIVOT_POS.y + attackColliderSize);

		pushOutPos = { pushedOutSize,0 };	// 밀려날 크기 설정
		pushCorrectionPos = { -1 ,0 };
		break;
	case CharacterInfo::UP:
		colliderRect.left = static_cast<LONG>(player->GetPos().x + PLAYER_PIVOT_POS.x - attackColliderSize);
		colliderRect.top = 	static_cast<LONG>(player->GetPos().y + PLAYER_PIVOT_POS.y - (static_cast<double>(attackColliderSize) * 2));
		colliderRect.right = static_cast<LONG>(player->GetPos().x + PLAYER_PIVOT_POS.x + attackColliderSize);
		colliderRect.bottom = static_cast<LONG>(player->GetPos().y + PLAYER_PIVOT_POS.y);

		pushOutPos = { 0, -pushedOutSize };	// 밀려날 크기 설정
		pushCorrectionPos = { 0 , 1 };
		break;
	case CharacterInfo::LEFT:
		colliderRect.left = static_cast<LONG>(player->GetPos().x + PLAYER_PIVOT_POS.x - (static_cast<double>(attackColliderSize) * 2));
		colliderRect.top = static_cast<LONG>(player->GetPos().y + PLAYER_PIVOT_POS.y - attackColliderSize);
		colliderRect.right = static_cast<LONG>(player->GetPos().x + PLAYER_PIVOT_POS.x );
		colliderRect.bottom = static_cast<LONG>(player->GetPos().y + PLAYER_PIVOT_POS.y + attackColliderSize);

		pushOutPos = { -pushedOutSize,0 };	// 밀려날 크기 설정
		pushCorrectionPos = { 1 ,0 };
		break;
	default:
		return;
	}

	for (auto& iterator: (*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()))
	{
		if (colliderRect.left <= iterator.GetPos().x + MONSTER1_PIVOT_POS.x &&
			colliderRect.right >= iterator.GetPos().x + MONSTER1_PIVOT_POS.x &&
			colliderRect.top <= iterator.GetPos().y + MONSTER1_PIVOT_POS.y &&
			colliderRect.bottom >= iterator.GetPos().y + MONSTER1_PIVOT_POS.y )
		{
			iterator.SetHp(iterator.GetHp() - player->GetDamage());		// 몬스터 hp -- 처리
			iterator.SetPos({ iterator.GetPos().x + pushOutPos.x , iterator.GetPos().y + pushOutPos.y });	// 밀려남 처리

			// 밀려남 처리 (맵 나가는거 보정)
			constexpr const int LIMIT_MAP_X_CORRECTION = 32;	// 맵 밖으로 나가는 경우 보정 크기
			constexpr const int LIMIT_MAP_Y_CORRECTION = 46;	// 맵 밖으로 나가는 경우 보정 크기

			if (0 > static_cast<int>(iterator.GetPos().x) || ClientSize::width - LIMIT_MAP_X_CORRECTION < static_cast<int>(iterator.GetPos().x))
			{
				iterator.SetPos({ static_cast<double>(ClientSize::width - LIMIT_MAP_X_CORRECTION) , iterator.GetPos().y});	// 밀려남 처리
				break;
			}
			if (0 > static_cast<int>(iterator.GetPos().y) || ClientSize::height - LIMIT_MAP_Y_CORRECTION < static_cast<int>(iterator.GetPos().y))
			{
				iterator.SetPos({ iterator.GetPos().x , static_cast<double>(ClientSize::height - LIMIT_MAP_Y_CORRECTION) });	// 밀려남 처리
				break;
			}

			// 맵 콜라이더 나가는거 보정
			POINT monsterMapPos;
			monsterMapPos.x = ((static_cast<int>(iterator.GetPos().x) + MONSTER1_PIVOT_POS.x)) / TILE_SIZE;
			monsterMapPos.y = ((static_cast<int>(iterator.GetPos().y) + MONSTER1_PIVOT_POS.y)) / TILE_SIZE;

			while (0 != WorldMapManager::GetInstance()->GetWorldMap()->GetData(SelectMapState::COLLIDER, monsterMapPos))
			{
				iterator.SetPos({ iterator.GetPos().x  + pushCorrectionPos.x, iterator.GetPos().y  + pushCorrectionPos .y});	// 밀려남 처리
				monsterMapPos.x = ((static_cast<int>(iterator.GetPos().x) + MONSTER1_PIVOT_POS.x)) / TILE_SIZE;
				monsterMapPos.y = ((static_cast<int>(iterator.GetPos().y) + MONSTER1_PIVOT_POS.y)) / TILE_SIZE;
			}
		}
	}
}

void GameManager::DieMonster()
{
	for (auto iterator = WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()->begin();
		iterator != WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()->end();)
	{
		if ((*iterator).GetHp() <= 0)
		{
			player->SetMoney(player->GetMoney() + (*iterator).GetMoney());
			player->SetExp((*iterator).GetExp());
			iterator = WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()->erase(iterator);
		}
		else
			iterator++;
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

Inventory* GameManager::GetInventory()
{
	return inventory;
}