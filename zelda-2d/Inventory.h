#pragma once
#include "Item.h"

constexpr const int INVEN_SIZE = 20;

class Inventory
{
private:
	Item item[INVEN_SIZE];
	
	bool isOpen;
public:
	Inventory();

	bool IsOpen();
	void SetOpen(const bool openState);
};

