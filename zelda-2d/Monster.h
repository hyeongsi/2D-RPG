#pragma once
#include "Character.h"
#include <vector>

using namespace std;

enum tile_data
{
	ROAD,
	WALL,
	START_LOCATION,
	END_LOCATION,
};

class ASNode;
class Monster : public Character
{
private:
	int index;
	int exp;

	friend class MonsterManager;
private:
	vector<ASNode*> openVec;	// astar openVector;
	vector<ASNode*> closeVec;	// astar closeVector;

	ASNode* startNode;			// aster startNode;
	ASNode* endNode;			// aster endNode;

	vector<ASNode*> resultVec;

	int tileMap[MAP_MAX_Y][MAP_MAX_X];	// astar tileMap;

public:
	Monster();
	Monster(const DPOINT pos, const int hp,  const int speed, const int damage, const int index, const int exp);

	const int GetIndex();
	void SetIndex(const int index);

	const int GetExp();

public:
	void initAstarNode();	// �ʱ�ȭ

	bool AttackCharacter(Character* character);	// ����
	void ChaseCharacter(Character* character);	// �߰�
	void FindPath();			// astar findPath;;			// �÷��̾� ����
	void SettingTileMap(Character * character);		// setting tileMap;
	void AddChildNode(const int childX, const int childY, ASNode* parentNode, const int value);
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
