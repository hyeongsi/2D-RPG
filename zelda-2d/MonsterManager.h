#pragma once
#include <vector>
#include <string>
#include "Monster.h"

using namespace std;

constexpr const char MONSTER_BITMAP_PATH[] = { "data/bitmapinfo/monster/MonsterBitmapInfoPath.txt" };
constexpr const char MONSTER_INFO_PATH[] = { "data/MonsterInfo/MonsterInfo.txt" };

class MonsterManager
{
private:
	vector<Monster> monsterData;

private:
	static MonsterManager* instance;

	MonsterManager();
	~MonsterManager();
public:
	static MonsterManager* GetInstance();
	static void ReleaseInstance();

public:
	vector<string> monsterBitmapPath;

public:
	vector<Monster> GetMonsterData();

	void LoadBitmapPath();
	void LoadMonsterData();

	const vector<string>* GetbitmapPath();

public:
	void FindPlayer(Monster* monster);			// 플레이어 감지
	void AttackPlayer(Monster* monster, const double deltaTime); // 플레이어를 공격
	void ChasePlayer(Monster* monster, const double deltaTime);	// 플레이어 추격

	void DieMonster();							// 몬스터 죽음 처리
	
};

