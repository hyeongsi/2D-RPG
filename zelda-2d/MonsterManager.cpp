#include "pch.h"
#include "MonsterManager.h"
#include "WorldMapManager.h"
#include "GameManager.h"
#include "SoundManager.h"
#include "RenderManager.h"
#include <iostream>
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