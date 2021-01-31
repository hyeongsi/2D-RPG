#include "pch.h"
#include "Monster.h"

Monster::Monster()
	:Character()
{
	index = 0;
	exp = 0;
}

Monster::Monster(const DPOINT pos, const int hp, const int speed, const int damage, const int index, const int exp)
	: Character(pos, hp, speed, damage)
{
	this->index = index;
	maxHp = hp;
	this->hp = hp;

	this->exp = exp;
}

void Monster::SetIndex(const int index)
{
	this->index = index;
}

const int Monster::GetExp()
{
	return exp;
}


