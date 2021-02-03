#include "pch.h"
#include "MonsterManager.h"
#include "WorldMapManager.h"
#include "GameManager.h"
#include "RenderManager.h"
#include <fstream>
#include <string>

MonsterManager* MonsterManager::instance = nullptr;

MonsterManager::MonsterManager()
{
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
		monster->SetState(CharacterInfo::ATTACK);
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
			hudData huddata;
			huddata.pos = { GameManager::GetInstance()->GetPlayer()->GetPos().x, 
				GameManager::GetInstance()->GetPlayer()->GetPos().y - 10 };
			huddata.msg = to_string(GameManager::GetInstance()->GetPlayer()->GetDamage());
			huddata.color = 0xff0000;
			RenderManager::GetInstance()->AddHudStringVector(huddata);

			GameManager::GetInstance()->PushOutPlayer(monster->GetDir());
		}
	}
}

void MonsterManager::ChasePlayer(Monster* monster)
{
	const int retouchPivotPos = 6;
	POINT diffPos;
	// x 값 비교
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

	// 대각선 이동 보정
	if (diffPos.x == 0 && diffPos.y == 0)
	{
		monster->SetPos({ monster->GetPos().x + ((monster->GetSpeed() * Timmer::GetInstance()->deltaTime) * diffPos.x) * sqrt(2) / 2 ,
		monster->GetPos().y + ((monster->GetSpeed() * Timmer::GetInstance()->deltaTime) * diffPos.y) * sqrt(2) / 2 });
	}
	else
	{
		monster->SetPos({ monster->GetPos().x + ((monster->GetSpeed() * Timmer::GetInstance()->deltaTime) * diffPos.x) ,
		monster->GetPos().y + ((monster->GetSpeed() * Timmer::GetInstance()->deltaTime) * diffPos.y) });
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
			hudData huddata;
			huddata.pos = { static_cast<double>(DRAW_MONEYINFO_UI_POS.x) + 100,
				static_cast<double>(DRAW_MONEYINFO_UI_POS.y) + 20 };
			huddata.msg = "+ " + to_string((*iterator).GetMoney());
			RenderManager::GetInstance()->AddHudStringVector(huddata);

			iterator = WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()->erase(iterator);
		}
		else
			iterator++;
	}
}