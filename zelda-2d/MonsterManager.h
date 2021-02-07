#pragma once
#include <vector>
#include <string>
#include "Monster.h"

using namespace std;

constexpr const char MONSTER_BITMAP_PATH[] = { "data/bitmapinfo/monster/MonsterBitmapInfoPath.txt" };
constexpr const char MONSTER_INFO_PATH[] = { "data/MonsterInfo/MonsterInfo.txt" };

enum tile_data
{
	ROAD,
	WALL,
	START_LOCATION,
	END_LOCATION,
};

class ASNode;
class MonsterManager
{
private:
	vector<Monster> monsterData;

private:
	vector<ASNode*> openVec;	// astar openVector;
	vector<ASNode*> closeVec;	// astar closeVector;

	ASNode* startNode;			// aster startNode;
	ASNode* endNode;			// aster endNode;

	vector<ASNode*> resultVec;

	int tileMap[MAP_MAX_Y][MAP_MAX_X];	// astar tileMap;
private:
	void FindPath();			// astar findPath;
	void SettingTileMap();		// setting tileMap;
	void AddChildNode(const int childX, const int childY, ASNode* parentNode, const int value);

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
	void AttackPlayer(Monster* monster);	// 플레이어를 공격
	void ChasePlayer(Monster* monster);		// 플레이어 추격

	void DieMonster();							// 몬스터 죽음 처리
};

class ASNode
{
public:
	int x, y;
	int f, g, h;

	ASNode* parentNode = nullptr;
public:
	ASNode(int _x, int _y)
	{
		x = _x;
		y = _y;
		f = 0;
		g = 0;
		h = 0;
	}
};

