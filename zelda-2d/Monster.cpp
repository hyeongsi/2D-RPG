#include "pch.h"
#include "Monster.h"
#include <math.h>
#include "Timmer.h"
#include "WorldMapManager.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include <string>

Monster::Monster()
	:Character()
{
	InitAstarNode();

	index = 0;
	exp = 0;
}

Monster::Monster(const DPOINT pos, const int hp, const int speed, const int damage, const int index, const int exp)
	: Character(pos, hp, speed, damage)
{
	InitAstarNode();

	this->index = index;
	maxHp = hp;
	this->hp = hp;

	this->exp = exp;
}

const int Monster::GetIndex()
{
	return index;
}

void Monster::SetIndex(const int index)
{
	this->index = index;
}

POINT Monster::GetPivotMapPoint()
{
	POINT pivotPos = { static_cast<LONG>(pos.x),  static_cast<LONG>(pos.y) };
	pivotPos.x += MONSTER1_PIVOT_POS.x;
	pivotPos.y += MONSTER1_PIVOT_POS.y;

	pivotPos.x /= TILE_SIZE;
	pivotPos.y /= TILE_SIZE;

	return pivotPos;
}

void Monster::InitAstarNode()
{
	startNode = nullptr;
	endNode = nullptr;

	for (int y = 0; y < MAP_MAX_Y; y++)
	{
		for (int x = 0; x < MAP_MAX_X; x++)
		{
			tileMap[y][x] = 0;
		}
	}
}

void Monster::InitStartEndNode(Character * character)
{
	// startNode : 본인 위치
	startNode = new ASNode(GetPivotMapPoint().x, GetPivotMapPoint().y);
	// endNode : 상대방 위치
	endNode = new ASNode(static_cast<int>((character->GetPos().x + PLAYER_PIVOT_POS.x)) / TILE_SIZE,
		static_cast<int>((character->GetPos().y + PLAYER_PIVOT_POS.y)) / TILE_SIZE);

	// startNode setting
	startNode->g = 0;
	startNode->h = abs((endNode->x - startNode->x)) + abs((endNode->y - startNode->y));
	startNode->f = startNode->g + startNode->h;

	openVec.emplace_back(new ASNode(startNode->x, startNode->y));
}

void Monster::InitAstarVector()
{
	// openVec 초기화
	for (auto iterator = openVec.begin(); iterator != openVec.end();)
	{
		delete* iterator;
		iterator = openVec.erase(iterator);
	}
	// closeVec 초기화
	for (auto iterator = closeVec.begin(); iterator != closeVec.end();)
	{
		delete* iterator;
		iterator = closeVec.erase(iterator);
	}
	resultVec.clear();

	delete endNode;
}

bool Monster::AttackCharacter(Character * character)
{
	// 공격 범위 안에 플레이어 접촉 상태
	if (pos.x + MONSTER1_PIVOT_POS.x - attackColliderSize <= character->GetPos().x + PLAYER_PIVOT_POS.x &&
		pos.x + MONSTER1_PIVOT_POS.x + attackColliderSize >= character->GetPos().x + PLAYER_PIVOT_POS.x &&
		pos.y + MONSTER1_PIVOT_POS.y - attackColliderSize <= character->GetPos().y + PLAYER_PIVOT_POS.y &&
		pos.y + MONSTER1_PIVOT_POS.y + attackColliderSize >= character->GetPos().y + PLAYER_PIVOT_POS.y)
	{
		if (GetTickCount64() > character->GetHitTick() + 1000)
		{
			character->SetHitTick(GetTickCount64());
			character->SetState(CharacterInfo::HIT);
			character->SetHp(character->GetHp() - damage);

			return true;
		}
	}

	return false;
}

void Monster::ChaseCharacter(Character* character)
{
	InitAstarVector();				// 초기화
	InitStartEndNode(character);
	SettingTileMap(character);		// 타일맵 세팅

	for (auto& iterator : (*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()))
	{
		// 본인이 아닐 때
		if (this == &iterator)
			continue;
		// 콜라이더가 겹치면
		if (pos.x + MONSTER1_PIVOT_POS.x - 16 <= iterator.GetPos().x + MONSTER1_PIVOT_POS.x &&
			pos.x + MONSTER1_PIVOT_POS.x + 16 >= iterator.GetPos().x + MONSTER1_PIVOT_POS.x &&
			pos.y + MONSTER1_PIVOT_POS.y - 16 <= iterator.GetPos().y + MONSTER1_PIVOT_POS.y &&
			pos.y + MONSTER1_PIVOT_POS.y + 16 >= iterator.GetPos().y + MONSTER1_PIVOT_POS.y)
		{
			// 이동방향에 벽을 생성해서 우회해서 갈수 있도록 함
			POINT diffDir[4] = { {0,1},{1,0},{0,-1},{-1,0} };
			tileMap[GetPivotMapPoint().y + diffDir[dir].y][GetPivotMapPoint().x + diffDir[dir].x] = WALL;
		}
	}

	FindPath();						// 경로 찾기
	FollowAstarAlgorithm(character);	// 경로 따라서 이동

	// 해결해야 할 문제 : 
	// 1. 다양한 이동관련 처리 도중 같은 타일에 적들이 2명이상 들어간 경우
	// 2. 적1 타일 오른쪽 끝자락 위치, 적2 타일 왼쪽 끝자락 위치 시 같이 이동하는 것 처럼 보이는 경우
	// -> 해결 했으나, 공격 처리를 못하는 문제가 발생, -> resultSize로, 해결해보던가, 1번을 해결하면 자연스럽게 해결될듯?
}

void Monster::FindPath()
{
	if (openVec.size() == 0)
		return;

	ASNode* currentNode = nullptr;

	// openVec 안에 있는 노드 중 f 값이 제일 값은 노드를 선택
	int smallest_f = 1000;
	for (auto& iterator : openVec)
	{
		if (iterator->f < smallest_f)
		{
			smallest_f = iterator->f;
			currentNode = iterator;
		}
	}

	if (currentNode != nullptr)
	{
		if (tileMap[currentNode->y][currentNode->x] == END_LOCATION)  // 현재 노드가 도착한 경우
		{
			// 길찾기 성공
			while (currentNode != nullptr)
			{
				resultVec.emplace_back(currentNode);
				currentNode = currentNode->parentNode;
			}

			return;
		}
		else	// 현재 노드가 도착하지 않았을 경우
		{
			ASNode* childNode = nullptr;
			POINT checkNode[8] = { {1,0},{0,1},{-1,0},{0,-1},{1,-1},{1,1},{-1,+1},{-1,-1} };
			for (int i = 0; i < 8; i++)
			{
				if (currentNode->x + checkNode[i].x < MAP_MAX_X && currentNode->y + checkNode[i].y < MAP_MAX_Y &&
					currentNode->x + checkNode[i].x >= 0 && currentNode->y + checkNode[i].y >= 0)	// 맵의 범위 안이라면
				{
					if (i < 4)
					{
						AddChildNode(currentNode->x + checkNode[i].x, currentNode->y + checkNode[i].y, currentNode, 10);
					}
					else if(tileMap[currentNode->y][currentNode->x + checkNode[i].x] != WALL && tileMap[currentNode->y + checkNode[i].y][currentNode->x] != WALL)
					{
						AddChildNode(currentNode->x + checkNode[i].x, currentNode->y + checkNode[i].y, currentNode, 14);
					}
				}
			}

			// 현재 노드를 열린노드에서 지우고 닫힌노드에 등록해 똑같은 노드를 조사하지 않도록 설정
			for (auto iterator = openVec.begin(); iterator != openVec.end();)
			{
				if ((*iterator)->x == currentNode->x && (*iterator)->y == currentNode->y)
				{
					openVec.erase(iterator);
					break;
				}
				else
					iterator++;
			}
			closeVec.emplace_back(currentNode);

			FindPath();
		}
	}
}

void Monster::AddChildNode(const int childX, const int childY, ASNode* parentNode, const int value)
{
	// 벽이면 종료
	if (tileMap[childY][childX] == WALL)
		return;

	for (auto& iterator : openVec)
	{
		if (iterator->x == childX && iterator->y == childY)
		{
			if (iterator->g > parentNode->g + value)
			{
				iterator->g = parentNode->g + value;
				iterator->parentNode = parentNode;
				iterator->f = iterator->g + iterator->h;
			}

			return;
		}
	}

	for (auto& iterator : closeVec)
	{
		if (iterator->x == childX && iterator->y == childY)
		{
			return;
		}
	}

	ASNode* childNode = new ASNode(childX, childY);
	childNode->parentNode = parentNode;
	childNode->g = parentNode->g + value;
	childNode->h = (abs((endNode->x - childNode->x)) + abs((endNode->y - childNode->y))) * 10;		// 도착점까지의 직선거리
	childNode->f = childNode->g + childNode->h;
	openVec.emplace_back(childNode);

}

void Monster::FollowAstarAlgorithm(Character* character)
{
	POINT diffPos;
	if (resultVec.size() == 0)
		return;

	if (resultVec.size() == 1)	// 같은 타일 위에 위치 한 경우
	{
		const int retouchPivotPos = 6;
		// x 값 비교
		if (static_cast<int>(character->GetPos().x) + PLAYER_PIVOT_POS.x >
			static_cast<int>(pos.x) + MONSTER1_PIVOT_POS.x)
			diffPos.x = 1;
		else if (static_cast<int>(character->GetPos().x) + PLAYER_PIVOT_POS.x <
			static_cast<int>(pos.x) + MONSTER1_PIVOT_POS.x)
			diffPos.x = -1;
		else
			diffPos.x = 0;

		// y 값 비교
		if (static_cast<int>(character->GetPos().y) + PLAYER_PIVOT_POS.y + retouchPivotPos >
			static_cast<int>(pos.y) + MONSTER1_PIVOT_POS.y)
			diffPos.y = 1;
		else if (static_cast<int>(character->GetPos().y) + PLAYER_PIVOT_POS.y + retouchPivotPos <
			static_cast<int>(pos.y) + MONSTER1_PIVOT_POS.y)
			diffPos.y = -1;
		else
			diffPos.y = 0;
	}
	else
	{
		diffPos.x = ((resultVec[resultVec.size() - 1]->x - (resultVec[resultVec.size() - 2])->x)) * -1;
		diffPos.y = ((resultVec[resultVec.size() - 1]->y - (resultVec[resultVec.size() - 2])->y)) * -1;
	}

	if (diffPos.x == 0 || diffPos.y == 0)
	{
		pos.x += (speed * Timmer::GetInstance()->deltaTime) * diffPos.x;
		pos.y += (speed * Timmer::GetInstance()->deltaTime) * diffPos.y;
	}
	else  // 대각선 이동 보정
	{

		pos.x += (speed * Timmer::GetInstance()->deltaTime) * diffPos.x * sqrt(2) / 2;
		pos.y += (speed * Timmer::GetInstance()->deltaTime) * diffPos.y * sqrt(2) / 2;
	}

	// 방향 설정
	if (diffPos.x > 0)
		dir = CharacterInfo::RIGHT;
	else if (diffPos.x < 0)
		dir = CharacterInfo::LEFT;
	else if (diffPos.y > 0)
		dir = CharacterInfo::DOWN;
	else if (diffPos.y < 0)
		dir = CharacterInfo::UP;
}

void Monster::Die(Character* character)
{
	for (auto iterator = WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()->begin();
		iterator != WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()->end();)
	{
		if ((*iterator).GetHp() <= 0)
		{
			character->SetMoney(character->GetMoney() + (*iterator).GetMoney());
			character->SetExp((*iterator).GetExp());

			RenderManager::GetInstance()->AddHudData(static_cast<double>(DRAW_MONEYINFO_UI_POS.x) + 130,
				static_cast<double>(DRAW_MONEYINFO_UI_POS.y) + 20,
				"+ " + std::to_string((*iterator).GetMoney()), 0x0000ff);

			SoundManager::GetInstance()->PlayEffectSound(EFFECTSOUND::SELL_ITEM);

			iterator = WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()->erase(iterator);
		}
		else
			iterator++;
	}
}

void Monster::SettingTileMap(Character * character)
{
	for (int y = 0; y < MAP_MAX_Y; y++)
	{
		for (int x = 0; x < MAP_MAX_X; x++)
		{
			tileMap[y][x] = WorldMapManager::GetInstance()->GetWorldMap()->GetData(SelectMapState::COLLIDER, { x,y });
		}
	}

	// monster pos init
	for (auto& iterator : (*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()))
	{
		if (this == &iterator)
			continue;
		tileMap[iterator.GetPivotMapPoint().y][iterator.GetPivotMapPoint().x] = WALL;
	}

	// player pos init
	tileMap[(static_cast<int>(character->GetPos().y) + PLAYER_PIVOT_POS.y) / TILE_SIZE]
		[(static_cast<int>(character->GetPos().x) + PLAYER_PIVOT_POS.x) / TILE_SIZE] = END_LOCATION;
}