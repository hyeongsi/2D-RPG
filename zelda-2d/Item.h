#pragma once
#include <string>

using namespace std;

class Item
{
private:
	int index;	// 아이템 인덱스
	int price;	// 아이템 가격

	string title;	// 아이템 제목
	string explain;	// 아이템 설명
public:
	friend class Inventory;
	friend class ItemManager;

	Item();
	Item(const int index, const int price);
	~Item();

	void SetIndex(const int index); 
	const int GetIndex();

	const string GetTitle();
	const string GetExplain();
	const int GetPrice();
};

