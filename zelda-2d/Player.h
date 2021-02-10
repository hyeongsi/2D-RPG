#pragma once
#include "Character.h"

class Player : public Character
{
private:
	int level;			// level

public:
	Player();
	Player(const DPOINT pos, const int hp, const int level, const int speed, const int damage);
	~Player();

public:
	const int GetLevel();

	POINT GetPivotMapPoint();

public:
	void Attack();						// 공격

public:
	void Input();
};

