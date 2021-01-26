#pragma once
#include "Character.h"
#include <vector>

using namespace std;

constexpr DPOINT NPC_START_POS = { 500,400 };

class ShopNPC : public Character
{
private:
	vector<int> sellItemId;	// 판매할 아이템 id

public:
	ShopNPC();
	~ShopNPC();

	void SetSellItemId(const int itemId);
};

