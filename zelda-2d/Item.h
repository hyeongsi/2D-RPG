#pragma once
#include <string>

using namespace std;

class Item
{
private:
	POINT pos;

	int index;	// 아이템 인덱스
	int price;	// 아이템 가격

	string title;	// 아이템 제목
	string explain;	// 아이템 설명
public:
	Item();
	Item(const POINT pos, const int index, const int price);
	~Item();

	void SetPos(const POINT pos);
	const POINT GetPos();

	void SetIndex(const int index);
	const int GetIndex();
};

