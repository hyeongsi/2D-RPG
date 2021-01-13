#pragma once
#include "DataStruct.h"

constexpr DPOINT START_POS = { 400,400 };
constexpr int MAX_HP = 6;
constexpr int MAX_LEVEL = 99;
constexpr int INIT_SPEED = 100;
constexpr int INIT_DAMAGE = 1;

class Character
{
private:
	DPOINT pos;			// 위치값

	int state;			// 상태값
	int dir;			// 방향

	int maxHp;			// maxHp
	int hp;				// hp

	int maxLevel;		// maxLevel
	int level;			// level

	int speed;			// speed
	int damage;			// damage

public:
	Character();
	Character(const DPOINT pos, const int hp, const int level, const int speed, const int damage);
	~Character();

public:
	const DPOINT GetPos();
	void SetPos(const DPOINT pos);

	const int GetHp();
	const int GetLevel();
	const int GetDamage();
	const int GetState();
	const int GetDir();

public:
	void Input(const double deltaTime);
	void Update();
};

