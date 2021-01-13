#include "pch.h"
#include "Character.h"

Character::Character()
{
	pos = START_POS;

	maxHp = MAX_HP;
	hp = MAX_HP;

	maxLevel = 99;
	level = 1;

	speed = INIT_SPEED;
	damage = INIT_DAMAGE;
}

Character::Character(const DPOINT pos, const int hp, const int level, const int speed, const int damage)
{
	this->pos = pos;

	maxHp = MAX_HP;
	this->hp = hp;

	maxLevel = 99;
	this->level = level;

	this->speed = speed;
	this->damage = damage;
}

Character::~Character()
{

}

const DPOINT Character::GetPos()
{
	return pos;
}

const int Character::GetHp()
{
	return hp;
}

const int Character::GetLevel()
{
	return level;
}

const int Character::GetDamage()
{
	return damage;
}

void Character::Input(const double deltaTime)
{
	// �̵� ���� �ڵ� �߰�

	if (GetAsyncKeyState(VK_UP) & 0x8000)		// ��
	{
		pos.y -= speed * deltaTime;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)		// ��
	{
		pos.y += speed * deltaTime;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)		// ��
	{
		pos.x -= speed * deltaTime;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	// ��
	{
		pos.x += speed * deltaTime;
	}
}

void Character::Update()
{

}
