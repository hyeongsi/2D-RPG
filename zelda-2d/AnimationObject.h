#pragma once

#include <vector>

using namespace std;

class AnimationObject
{
private:
	vector<HBITMAP> bitmapData;				
	vector<POINT> normalPos;				

public:
	void AddBitmap(HBITMAP hbitmap);

	const HBITMAP GetAnimationBitmap(const int index);
	void SetBitmap(const int index, const HBITMAP hbitmap);

	const POINT GetNormalPos(const int index);
	void SetNormalPos(const int index, const POINT pos);
};
