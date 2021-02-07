#include "pch.h"
#include "MonsterManager.h"
#include "WorldMapManager.h"
#include "GameManager.h"
#include "SoundManager.h"
#include "RenderManager.h"
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

MonsterManager* MonsterManager::instance = nullptr;

void MonsterManager::FindPath()
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

			if (resultVec.size() != 1)
				return;
		}
		else	// 현재 노드가 도착하지 않았을 경우
		{
			ASNode* childNode = nullptr;
			if (currentNode->x + 1 < MAP_MAX_X)	// 오른쪽
			{
				AddChildNode(currentNode->x + 1, currentNode->y, currentNode, 10);
			}

			if (currentNode->y + 1 < MAP_MAX_Y)	// 아래
			{
				AddChildNode(currentNode->x, currentNode->y + 1, currentNode, 10);
			}

			if (currentNode->x - 1 >= 0)	// 왼쪽
			{
				AddChildNode(currentNode->x - 1, currentNode->y, currentNode, 10);
			}

			if (currentNode->y - 1 >= 0)	// 위
			{
				AddChildNode(currentNode->x, currentNode->y - 1, currentNode, 10);
			}

			if (currentNode->x + 1 < MAP_MAX_X && currentNode->y - 1 >= 0)	// 1사분면
			{
				if (tileMap[currentNode->y][currentNode->x + 1] != WALL && tileMap[currentNode->y - 1][currentNode->x] != WALL)
					AddChildNode(currentNode->x + 1, currentNode->y - 1, currentNode, 14);
			}

			if (currentNode->x + 1 && currentNode->y + 1 < MAP_MAX_Y)	// 4사분면
			{
				if (tileMap[currentNode->y][currentNode->x + 1] != WALL && tileMap[currentNode->y + 1][currentNode->x] != WALL)
					AddChildNode(currentNode->x + 1, currentNode->y + 1, currentNode, 14);
			}

			if (currentNode->x - 1 >= 0 && currentNode->y + 1 >= 0)	// 3사분면
			{
				if (tileMap[currentNode->y][currentNode->x - 1] != WALL && tileMap[currentNode->y + 1][currentNode->x] != WALL)
					AddChildNode(currentNode->x - 1, currentNode->y + 1, currentNode, 14);
			}

			if (currentNode->x - 1 >= 0 && currentNode->y - 1 >= 0)	 // 2사분면
			{
				if (tileMap[currentNode->y][currentNode->x - 1] != WALL && tileMap[currentNode->y - 1][currentNode->x] != WALL)
					AddChildNode(currentNode->x - 1, currentNode->y - 1, currentNode, 14);
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

void MonsterManager::AddChildNode(const int childX, const int childY, ASNode* parentNode, const int value)
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

};

void MonsterManager::SettingTileMap()
{
	for (int y = 0; y < MAP_MAX_Y; y++)
	{
		for (int x = 0; x < MAP_MAX_X; x++)
		{
			tileMap[y][x] = WorldMapManager::GetInstance()->GetWorldMap()->GetData(SelectMapState::COLLIDER, { x,y });
		}
	}

	// monster pos init
	int x, y;
	for (auto& iterator : (*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()))
	{
		x = ((static_cast<int>(iterator.GetPos().x) + MONSTER1_PIVOT_POS.x) / TILE_SIZE);
		y = ((static_cast<int>(iterator.GetPos().y) + MONSTER1_PIVOT_POS.y) / TILE_SIZE);

		tileMap[y][x] = WALL;
	}

	// player pos init
	tileMap[GameManager::GetInstance()->GetPlayerPivotMapPoint().y][GameManager::GetInstance()->GetPlayerPivotMapPoint().x] = END_LOCATION;
}

MonsterManager::MonsterManager()
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

MonsterManager::~MonsterManager()
{
}

MonsterManager* MonsterManager::GetInstance()
{
	if (nullptr == instance)
		instance = new MonsterManager();

	return instance;
}

void MonsterManager::ReleaseInstance()
{
	delete instance;
	instance = nullptr;
}

vector<Monster> MonsterManager::GetMonsterData()
{
	return monsterData;
}

void MonsterManager::LoadBitmapPath()
{
	ifstream readFile;
	string str;

	try
	{
		readFile.open(MONSTER_BITMAP_PATH);

		if (readFile.is_open())
		{
			while (!readFile.eof())
			{
				readFile >> str;	// index
				readFile >> str;	// path
				monsterBitmapPath.emplace_back(str);
			}
		}
	}
	catch (const std::exception&)
	{
	}

	readFile.close();
}

void MonsterManager::LoadMonsterData()
{
	const int explainTextSize = 6;
	ifstream readFile;
	string str;
	Monster monster;

	try
	{
		readFile.open(MONSTER_INFO_PATH);
		
		for (int i = 0; i < explainTextSize; i++)
		{
			readFile >> str;	// 설명 6개 처리
		}

		if (readFile.is_open())
		{
			while (!readFile.eof())
			{
				readFile >> str;	// id
				monster.index = stoi(str);	// index
				readFile >> str;	// maxHp
				monster.maxHp = stoi(str);
				monster.hp = monster.maxHp;
				readFile >> str;	// speed
				monster.speed = stoi(str);
				readFile >> str;	// damage
				monster.damage = stoi(str);
				readFile >> str;	// exp
				monster.exp = stoi(str);
				readFile >> str;	// money
				monster.money = stoi(str);

				monsterData.emplace_back(monster);
			}
		}
	}
	catch (const std::exception&)
	{
	}

	readFile.close();
}

const vector<string>* MonsterManager::GetbitmapPath()
{
	return &monsterBitmapPath;
}

void MonsterManager::FindPlayer(Monster* monster)
{
	const int detectSize = 100;
	RECT findRect = { {static_cast<LONG>(monster->GetPos().x + MONSTER1_PIVOT_POS.x - detectSize)},
		{static_cast<LONG>(monster->GetPos().y + MONSTER1_PIVOT_POS.y - detectSize)},
		{static_cast<LONG>(monster->GetPos().x + MONSTER1_PIVOT_POS.x + detectSize)},
		{static_cast<LONG>(monster->GetPos().y + MONSTER1_PIVOT_POS.y + detectSize)} };

	// 플레이어의 위치가 감지범위 내에 왔을경우
	if (findRect.left <= GameManager::GetInstance()->GetPlayer()->GetPos().x + PLAYER_PIVOT_POS.x &&
		findRect.top <= GameManager::GetInstance()->GetPlayer()->GetPos().y + PLAYER_PIVOT_POS.y &&
		findRect.right >= GameManager::GetInstance()->GetPlayer()->GetPos().x + PLAYER_PIVOT_POS.x &&
		findRect.bottom >= GameManager::GetInstance()->GetPlayer()->GetPos().y + PLAYER_PIVOT_POS.y)
	{
		monster->SetState(CharacterInfo::ATTACK);	// 몬스터를 공격 상태로 변경
	}
}

void MonsterManager::AttackPlayer(Monster* monster)
{
	const int attackColliderSize = 13;
	// 공격 범위 안에 플레이어 접촉 상태
	if (monster->GetPos().x + MONSTER1_PIVOT_POS.x - attackColliderSize <= GameManager::GetInstance()->GetPlayer()->GetPos().x + PLAYER_PIVOT_POS.x &&
		monster->GetPos().x + MONSTER1_PIVOT_POS.x + attackColliderSize >= GameManager::GetInstance()->GetPlayer()->GetPos().x + PLAYER_PIVOT_POS.x &&
		monster->GetPos().y + MONSTER1_PIVOT_POS.y - attackColliderSize <= GameManager::GetInstance()->GetPlayer()->GetPos().y + PLAYER_PIVOT_POS.y &&
		monster->GetPos().y + MONSTER1_PIVOT_POS.y + attackColliderSize >= GameManager::GetInstance()->GetPlayer()->GetPos().y + PLAYER_PIVOT_POS.y)
	{
		if (GetTickCount64() > GameManager::GetInstance()->playerHitTick + 1000)
		{
			GameManager::GetInstance()->playerHitTick = GetTickCount64();
			GameManager::GetInstance()->GetPlayer()->SetState(CharacterInfo::HIT);
			GameManager::GetInstance()->GetPlayer()->SetHp(
				GameManager::GetInstance()->GetPlayer()->GetHp() - monster->GetDamage());

			// hud 출력 관련 데이터 세팅
			textHudData hudData;
			hudData.pos = { GameManager::GetInstance()->GetPlayer()->GetPos().x, 
				GameManager::GetInstance()->GetPlayer()->GetPos().y - 10 };
			hudData.msg = to_string(monster->GetDamage());
			hudData.color = 0xff0000;
			RenderManager::GetInstance()->GetHud()->GetStringHud()->emplace_back(hudData);

			SoundManager::GetInstance()->PlayEffectSound(EFFECTSOUND::HIT);
			GameManager::GetInstance()->PushOutPlayer(monster->GetDir());
		}
	}
}

void MonsterManager::ChasePlayer(Monster* monster)
{
	// openVec 초기화
	for (auto iterator = openVec.begin(); iterator != openVec.end();)
	{
		delete *iterator;
		iterator = openVec.erase(iterator);
	}
	// closeVec 초기화
	for (auto iterator = closeVec.begin(); iterator != closeVec.end();)
	{
		delete* iterator;
		iterator = closeVec.erase(iterator);
	}
	resultVec.clear();

	// startNode : moster 위치
	startNode = new ASNode((static_cast<int>(monster->GetPos().x) + MONSTER1_PIVOT_POS.x) / TILE_SIZE, 
		(static_cast<int>(monster->GetPos().y) + MONSTER1_PIVOT_POS.y) / TILE_SIZE);
	// endNode : player위치
	endNode = new ASNode(GameManager::GetInstance()->GetPlayerPivotMapPoint().x, GameManager::GetInstance()->GetPlayerPivotMapPoint().y);

	// startNode setting
	startNode->g = 0;
	startNode->h = abs((endNode->x - startNode->x)) + abs((endNode->y - startNode->y));
	startNode->f = startNode->g + startNode->h;

	openVec.emplace_back(new ASNode(startNode->x, startNode->y));

	SettingTileMap();

	FindPath();		// astar findPath

	if (resultVec.size() < 1)
		return;

	POINT diffPos;

	if (resultVec.size() == 1)	// 같은 타일 위에 위치 한 경우
	{
		const int retouchPivotPos = 6;

		if (static_cast<int>(GameManager::GetInstance()->GetPlayer()->GetPos().x) + PLAYER_PIVOT_POS.x >
			static_cast<int>(monster->GetPos().x) + MONSTER1_PIVOT_POS.x)
			diffPos.x = 1;
		else if (static_cast<int>(GameManager::GetInstance()->GetPlayer()->GetPos().x) + PLAYER_PIVOT_POS.x <
			static_cast<int>(monster->GetPos().x) + MONSTER1_PIVOT_POS.x)
			diffPos.x = -1;
		else
			diffPos.x = 0;

		// y 값 비교
		if (static_cast<int>(GameManager::GetInstance()->GetPlayer()->GetPos().y) + PLAYER_PIVOT_POS.y + retouchPivotPos >
			static_cast<int>(monster->GetPos().y) + MONSTER1_PIVOT_POS.y)
			diffPos.y = 1;
		else if (static_cast<int>(GameManager::GetInstance()->GetPlayer()->GetPos().y) + PLAYER_PIVOT_POS.y + retouchPivotPos <
			static_cast<int>(monster->GetPos().y) + MONSTER1_PIVOT_POS.y)
			diffPos.y = -1;
		else
			diffPos.y = 0;
	}
	else
	{
		diffPos.x = ((resultVec[resultVec.size() - 1]->x - (resultVec[resultVec.size() - 2])->x)) * -1;
		diffPos.y = ((resultVec[resultVec.size() - 1]->y - (resultVec[resultVec.size() - 2])->y)) * -1;
	}

	// 대각선 이동 보정
	if (diffPos.x == 0 || diffPos.y == 0)
	{
		monster->SetPos({ monster->GetPos().x + ((monster->GetSpeed() * Timmer::GetInstance()->deltaTime) * diffPos.x) ,
		monster->GetPos().y + ((monster->GetSpeed() * Timmer::GetInstance()->deltaTime) * diffPos.y) });
	}
	else
	{
		monster->SetPos({ monster->GetPos().x + ((monster->GetSpeed() * Timmer::GetInstance()->deltaTime) * diffPos.x) * sqrt(2) / 2 ,
		monster->GetPos().y + ((monster->GetSpeed() * Timmer::GetInstance()->deltaTime) * diffPos.y) * sqrt(2) / 2 });
	}

	// 방향 설정
	if (diffPos.x > 0)
		monster->SetDir(CharacterInfo::RIGHT);
	else if (diffPos.x < 0)
		monster->SetDir(CharacterInfo::LEFT);
	else if (diffPos.y > 0)
		monster->SetDir(CharacterInfo::DOWN);
	else if (diffPos.y < 0)
		monster->SetDir(CharacterInfo::UP);
}

void MonsterManager::DieMonster()
{
	for (auto iterator = WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()->begin();
		iterator != WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()->end();)
	{
		if ((*iterator).GetHp() <= 0)
		{
			GameManager::GetInstance()->GetPlayer()->SetMoney(GameManager::GetInstance()->GetPlayer()->GetMoney() + (*iterator).GetMoney());
			GameManager::GetInstance()->GetPlayer()->SetExp((*iterator).GetExp());

			// hud 출력 관련 데이터 세팅
			textHudData hudData;
			hudData.pos = { static_cast<double>(DRAW_MONEYINFO_UI_POS.x) + 130,
				static_cast<double>(DRAW_MONEYINFO_UI_POS.y) + 20 };
			hudData.msg = "+ " + to_string((*iterator).GetMoney());
			RenderManager::GetInstance()->GetHud()->GetStringHud()->emplace_back(hudData);

			SoundManager::GetInstance()->PlayEffectSound(EFFECTSOUND::SELL_ITEM);

			iterator = WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()->erase(iterator);
		}
		else
			iterator++;
	}
}