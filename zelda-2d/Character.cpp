#include "pch.h"
#include "Character.h"

Character::Character()
{
	pos = START_POS;

	state = CharacterInfo::IDLE;
	dir = CharacterInfo::DOWN;

	maxHp = INIT_HP;
	hp = maxHp;

	speed = INIT_SPEED;
	damage = INIT_DAMAGE;

	money = 0;
	exp = 0;

	hitTick = GetTickCount64();
	attackTick = GetTickCount64();
}

Character::Character(const DPOINT pos, const int hp, const int speed, const int damage)
{
	this->pos = pos;

	state = CharacterInfo::IDLE;
	dir = CharacterInfo::DOWN;

	maxHp = hp;
	this->hp = hp;

	this->speed = speed;
	this->damage = damage;

	money = 0;
	exp = 0;

	hitTick = GetTickCount64();
	attackTick = GetTickCount64();
}

Character::~Character()
{
}

const DPOINT Character::GetPos()
{
	return pos;
}

void Character::SetPos(const DPOINT pos)
{
	this->pos = pos;
}

const int Character::GetMaxHp()
{
	return maxHp;
}

const int Character::GetHp()
{
	return hp;
}

void Character::SetHp(const int hp)
{
	if (hp >= maxHp)
		this->hp = maxHp;
	else
		this->hp = hp;
}

const int Character::GetSpeed()
{
	return speed;
}

const int Character::GetDamage()
{
	return damage;
}

void Character::SetDamage(const int damage)
{
	this->damage = damage;
}

const int Character::GetState()
{
	return state;
}

void Character::SetState(const int state)
{
	this->state = state;
}

const int Character::GetDir()
{
	return dir;
}

void Character::SetDir(const int dir)
{
	this->dir = dir;
}

const int Character::GetMoney()
{
	return money;
}

void Character::SetMoney(const int money)
{
	this->money = money;
}

void Character::SetExp(const int exp)
{
	this->exp = exp;
}

const int Character::GetExp()
{
	return exp;
}

const ULONGLONG Character::GetHitTick()
{
	return hitTick;
}

void Character::SetHitTick(const ULONGLONG hitTick)
{
	this->hitTick = hitTick;
}

const ULONGLONG Character::GetAttackTick()
{
	return attackTick;
}

void Character::SetAttackTick(const ULONGLONG attackTick)
{
	this->attackTick = attackTick;
}
