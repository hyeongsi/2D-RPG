#include "pch.h"
#include "Monster.h"
#include <math.h>
#include "Timmer.h"
#include "WorldMapManager.h"

Monster::Monster()
	:Character()
{
	initAstarNode();

	index = 0;
	exp = 0;
}

Monster::Monster(const DPOINT pos, const int hp, const int speed, const int damage, const int index, const int exp)
	: Character(pos, hp, speed, damage)
{
	initAstarNode();

	this->index = index;
	maxHp = hp;
	this->hp = hp;

	this->exp = exp;
}

const int Monster::GetIndex()
{
	return index;
}

void Monster::SetIndex(const int index)
{
	this->index = index;
}

const int Monster::GetExp()
{
	return exp;
}

void Monster::initAstarNode()
{
	startNode = nullptr;
	endNode = nullptr;

	for (int y = 0; y < MAP_MAX_Y; y++)
	{
		for (int x = 0; x < MAP_MAX_X; x++)
		{
			tileMap[y][x] = 0;
		}
	}
}

bool Monster::AttackCharacter(Character * character)
{
	const int attackColliderSize = 13;
	// ���� ���� �ȿ� �÷��̾� ���� ����
	if (pos.x + MONSTER1_PIVOT_POS.x - attackColliderSize <= character->GetPos().x + PLAYER_PIVOT_POS.x &&
		pos.x + MONSTER1_PIVOT_POS.x + attackColliderSize >= character->GetPos().x + PLAYER_PIVOT_POS.x &&
		pos.y + MONSTER1_PIVOT_POS.y - attackColliderSize <= character->GetPos().y + PLAYER_PIVOT_POS.y &&
		pos.y + MONSTER1_PIVOT_POS.y + attackColliderSize >= character->GetPos().y + PLAYER_PIVOT_POS.y)
	{
		if (GetTickCount64() > character->GetHitTick() + 1000)
		{
			character->SetHitTick(GetTickCount64());
			character->SetState(CharacterInfo::HIT);
			character->SetHp(character->GetHp() - damage);

			return true;
		}
	}

	return false;
}

void Monster::ChaseCharacter(Character* character)
{
	// openVec �ʱ�ȭ
	for (auto iterator = openVec.begin(); iterator != openVec.end();)
	{
		delete* iterator;
		iterator = openVec.erase(iterator);
	}
	// closeVec �ʱ�ȭ
	for (auto iterator = closeVec.begin(); iterator != closeVec.end();)
	{
		delete* iterator;
		iterator = closeVec.erase(iterator);
	}
	resultVec.clear();

	// startNode : ���� ��ġ
	startNode = new ASNode((static_cast<int>(pos.x) + MONSTER1_PIVOT_POS.x) / TILE_SIZE,
		(static_cast<int>(pos.y) + MONSTER1_PIVOT_POS.y) / TILE_SIZE);
	// endNode : ���� ��ġ
	endNode = new ASNode(static_cast<int>((character->GetPos().x + PLAYER_PIVOT_POS.x)) / TILE_SIZE,
		static_cast<int>((character->GetPos().y + PLAYER_PIVOT_POS.y)) / TILE_SIZE);

	// startNode setting
	startNode->g = 0;
	startNode->h = abs((endNode->x - startNode->x)) + abs((endNode->y - startNode->y));
	startNode->f = startNode->g + startNode->h;

	openVec.emplace_back(new ASNode(startNode->x, startNode->y));

	SettingTileMap(character);

	FindPath();		// astar findPath

	if (resultVec.size() < 1)
		return;

	POINT diffPos;

	if (resultVec.size() == 1)	// ���� Ÿ�� ���� ��ġ �� ���
	{
		const int retouchPivotPos = 6;
		// x �� ��
		if (static_cast<int>(character->GetPos().x) + PLAYER_PIVOT_POS.x >
			static_cast<int>(pos.x) + MONSTER1_PIVOT_POS.x)
			diffPos.x = 1;
		else if (static_cast<int>(character->GetPos().x) + PLAYER_PIVOT_POS.x <
			static_cast<int>(pos.x) + MONSTER1_PIVOT_POS.x)
			diffPos.x = -1;
		else
			diffPos.x = 0;

		// y �� ��
		if (static_cast<int>(character->GetPos().y) + PLAYER_PIVOT_POS.y + retouchPivotPos >
			static_cast<int>(pos.y) + MONSTER1_PIVOT_POS.y)
			diffPos.y = 1;
		else if (static_cast<int>(character->GetPos().y) + PLAYER_PIVOT_POS.y + retouchPivotPos <
			static_cast<int>(pos.y) + MONSTER1_PIVOT_POS.y)
			diffPos.y = -1;
		else
			diffPos.y = 0;
	}
	else
	{
		diffPos.x = ((resultVec[resultVec.size() - 1]->x - (resultVec[resultVec.size() - 2])->x)) * -1;
		diffPos.y = ((resultVec[resultVec.size() - 1]->y - (resultVec[resultVec.size() - 2])->y)) * -1;
	}

	// �밢�� �̵� ����
	if (diffPos.x == 0 || diffPos.y == 0)
	{
		pos.x +=  (speed * Timmer::GetInstance()->deltaTime) * diffPos.x;
		pos.y += (speed * Timmer::GetInstance()->deltaTime) * diffPos.y;
	}
	else
	{
		pos.x += (speed * Timmer::GetInstance()->deltaTime) * diffPos.x * sqrt(2) / 2;
		pos.y += (speed * Timmer::GetInstance()->deltaTime) * diffPos.y * sqrt(2) / 2;
	}

	// ���� ����
	if (diffPos.x > 0)
		dir = CharacterInfo::RIGHT;
	else if (diffPos.x < 0)
		dir = CharacterInfo::LEFT;
	else if (diffPos.y > 0)
		dir = CharacterInfo::DOWN;
	else if (diffPos.y < 0)
		dir = CharacterInfo::UP;
}

void Monster::FindPath()
{
	if (openVec.size() == 0)
		return;

	ASNode* currentNode = nullptr;

	// openVec �ȿ� �ִ� ��� �� f ���� ���� ���� ��带 ����
	int smallest_f = 1000;
	for (auto& iterator : openVec)
	{
		if (iterator->f < smallest_f)
		{
			smallest_f = iterator->f;
			currentNode = iterator;
		}
	}

	if (currentNode != nullptr)
	{
		if (tileMap[currentNode->y][currentNode->x] == END_LOCATION)  // ���� ��尡 ������ ���
		{
			// ��ã�� ����
			while (currentNode != nullptr)
			{
				resultVec.emplace_back(currentNode);
				currentNode = currentNode->parentNode;
			}

			if (resultVec.size() != 1)
				return;
		}
		else	// ���� ��尡 �������� �ʾ��� ���
		{
			ASNode* childNode = nullptr;
			if (currentNode->x + 1 < MAP_MAX_X)	// ������
			{
				AddChildNode(currentNode->x + 1, currentNode->y, currentNode, 10);
			}

			if (currentNode->y + 1 < MAP_MAX_Y)	// �Ʒ�
			{
				AddChildNode(currentNode->x, currentNode->y + 1, currentNode, 10);
			}

			if (currentNode->x - 1 >= 0)	// ����
			{
				AddChildNode(currentNode->x - 1, currentNode->y, currentNode, 10);
			}

			if (currentNode->y - 1 >= 0)	// ��
			{
				AddChildNode(currentNode->x, currentNode->y - 1, currentNode, 10);
			}

			if (currentNode->x + 1 < MAP_MAX_X && currentNode->y - 1 >= 0)	// 1��и�
			{
				if (tileMap[currentNode->y][currentNode->x + 1] != WALL && tileMap[currentNode->y - 1][currentNode->x] != WALL)
					AddChildNode(currentNode->x + 1, currentNode->y - 1, currentNode, 14);
			}

			if (currentNode->x + 1 && currentNode->y + 1 < MAP_MAX_Y)	// 4��и�
			{
				if (tileMap[currentNode->y][currentNode->x + 1] != WALL && tileMap[currentNode->y + 1][currentNode->x] != WALL)
					AddChildNode(currentNode->x + 1, currentNode->y + 1, currentNode, 14);
			}

			if (currentNode->x - 1 >= 0 && currentNode->y + 1 >= 0)	// 3��и�
			{
				if (tileMap[currentNode->y][currentNode->x - 1] != WALL && tileMap[currentNode->y + 1][currentNode->x] != WALL)
					AddChildNode(currentNode->x - 1, currentNode->y + 1, currentNode, 14);
			}

			if (currentNode->x - 1 >= 0 && currentNode->y - 1 >= 0)	 // 2��и�
			{
				if (tileMap[currentNode->y][currentNode->x - 1] != WALL && tileMap[currentNode->y - 1][currentNode->x] != WALL)
					AddChildNode(currentNode->x - 1, currentNode->y - 1, currentNode, 14);
			}

			// ���� ��带 ������忡�� ����� ������忡 ����� �Ȱ��� ��带 �������� �ʵ��� ����
			for (auto iterator = openVec.begin(); iterator != openVec.end();)
			{
				if ((*iterator)->x == currentNode->x && (*iterator)->y == currentNode->y)
				{
					openVec.erase(iterator);
					break;
				}
				else
					iterator++;
			}
			closeVec.emplace_back(currentNode);

			FindPath();
		}
	}
}

void Monster::AddChildNode(const int childX, const int childY, ASNode* parentNode, const int value)
{
	// ���̸� ����
	if (tileMap[childY][childX] == WALL)
		return;

	for (auto& iterator : openVec)
	{
		if (iterator->x == childX && iterator->y == childY)
		{
			if (iterator->g > parentNode->g + value)
			{
				iterator->g = parentNode->g + value;
				iterator->parentNode = parentNode;
				iterator->f = iterator->g + iterator->h;
			}

			return;
		}
	}

	for (auto& iterator : closeVec)
	{
		if (iterator->x == childX && iterator->y == childY)
		{
			return;
		}
	}

	ASNode* childNode = new ASNode(childX, childY);
	childNode->parentNode = parentNode;
	childNode->g = parentNode->g + value;
	childNode->h = (abs((endNode->x - childNode->x)) + abs((endNode->y - childNode->y))) * 10;		// ������������ �����Ÿ�
	childNode->f = childNode->g + childNode->h;
	openVec.emplace_back(childNode);

};

void Monster::SettingTileMap(Character * character)
{
	for (int y = 0; y < MAP_MAX_Y; y++)
	{
		for (int x = 0; x < MAP_MAX_X; x++)
		{
			tileMap[y][x] = WorldMapManager::GetInstance()->GetWorldMap()->GetData(SelectMapState::COLLIDER, { x,y });
		}
	}

	// monster pos init
	int x, y;
	for (auto& iterator : (*WorldMapManager::GetInstance()->GetWorldMap()->GetMonsterData()))
	{
		x = ((static_cast<int>(iterator.GetPos().x) + MONSTER1_PIVOT_POS.x) / TILE_SIZE);
		y = ((static_cast<int>(iterator.GetPos().y) + MONSTER1_PIVOT_POS.y) / TILE_SIZE);

		tileMap[y][x] = WALL;
	}

	// player pos init
	tileMap[(static_cast<int>(character->GetPos().y) + PLAYER_PIVOT_POS.y) / TILE_SIZE]
		[(static_cast<int>(character->GetPos().x) + PLAYER_PIVOT_POS.x) / TILE_SIZE] = END_LOCATION;
}