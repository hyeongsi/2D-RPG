#include "pch.h"
#include "Inventory.h"

Inventory::Inventory()
{
	for (int i = 0; i < INVEN_SIZE; i++)
	{
		item[i].index = 0;
	}

	itemLastIndex = 0;
	isOpen = false;
}

bool Inventory::IsOpen()
{
	return isOpen;
}

void Inventory::SetOpen(const bool openState)
{
	isOpen = openState;
}

void Inventory::SetItem(Item item)
{
	this->item[itemLastIndex] = item;
	itemLastIndex++;
}

void Inventory::DeleteItem(const int index)
{
	if (0 <= index && index < INVEN_SIZE)
	{
		item[itemLastIndex--].index = 0;

		for (int i = index; i < itemLastIndex; i++)
			item[i] = item[i + 1];
	}
}

void Inventory::SwitchingItem(const int index1, const int index2)
{
	Item tempItem;
	tempItem = item[index1];

	item[index1] = item[index2];
	item[index2] = tempItem;
}

Item* Inventory::GetItem()
{
	return item;
}

int Inventory::GetLastItemIndex()
{
	return itemLastIndex;
}
