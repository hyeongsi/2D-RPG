#include "pch.h"
#include "Inventory.h"

Inventory::Inventory()
{
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
