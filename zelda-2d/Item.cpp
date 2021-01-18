#include "pch.h"
#include "Item.h"

Item::Item()
{
	pos = { NULL, NULL };

	index = NULL;
	price = NULL;
}

Item::Item(const POINT pos, const int index, const int price)
{
	this->pos = pos;
	this->index = index;
	this->price = price;
}

Item::~Item()
{
}

void Item::SetPos(const POINT pos)
{
	this->pos = pos;
}

const POINT Item::GetPos()
{
	return pos;
}

void Item::SetIndex(const int index)
{
	this->index = index;
}

const int Item::GetIndex()
{
	return index;
}
