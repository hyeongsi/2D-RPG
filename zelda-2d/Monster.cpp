#include "pch.h"
#include "Monster.h"

Monster::Monster()
	:Character()
{
}

Monster::Monster(const DPOINT pos, const int hp, const int speed, const int damage, const int exp)
	: Character(pos, hp, speed, damage)
{
	maxHp = hp;
	this->hp = hp;

	this->exp = exp;
}

const int Monster::GetExp()
{
	return exp;
}


