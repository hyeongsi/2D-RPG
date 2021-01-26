#include "pch.h"
#include "ShopNPC.h"

ShopNPC::ShopNPC()
{
	pos = NPC_START_POS;
}

ShopNPC::~ShopNPC()
{
}

void ShopNPC::SetSellItemId(const int itemId)
{
	sellItemId.emplace_back(itemId);
}
