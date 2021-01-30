#pragma once
#include <vector>
#include "Monster.h"

using namespace std;

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
	void LoadMonsterData();
};

