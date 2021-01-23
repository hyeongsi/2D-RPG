﻿#pragma once
#include "Item.h"

constexpr const SIZE INVENTORY_SIZE = { 128,208 };
constexpr const int INVEN_SIZE = 16;

class Inventory
{
private:
	Item item[INVEN_SIZE];
	int itemLastIndex;
	
	bool isOpen;
public:
	Inventory();

	bool IsOpen();
	void SetOpen(const bool openState);

	void SetItem(Item item);
	void DeleteItem(const int index);

	Item* GetItem();

	int GetLastItemIndex();
};

