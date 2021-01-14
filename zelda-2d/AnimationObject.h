#pragma once

#include <vector>

using namespace std;

class AnimationObject
{
private:
	vector<HBITMAP> animationBitmapData; // 애니메이션 비트맵	
	vector<int> bitmapCount;	// 한 애니메이션 속에서 나눠쓸 비트맵 수 (한 비트맵 안의 짤라서 사용할 비트맵 수)

	vector<int> delay;		// 애니메이션 변경 딜레이
	ULONGLONG tick = GetTickCount64();

	int selectAnimationBitmapIndex = 0;			// 출력할 애니메이션 비트맵 인덱스
	int selectBitmapIndex = 0;					// 출력할 비트맵 인덱스

public:
	void AddAnimationBitmap(const HBITMAP hbitmap);
	void AddBitmapCount(const int count);
	void AddNormalPos(const POINT pos);
	void AddDelay(const int delay);

public:
	const int GetAnimationBitmapSize();
	const int GetBitmapCount(const int index);

	const int GetSelectAnimationBitmapIndex();
	void SetSelectAnimationBitmapIndex(const int index);

	const int GetSelectBitmapIndex();
	void SetSelectBitmapIndex(const int index);

	const HBITMAP GetAnimationBitmap(const int index);
	void SetBitmap(const int index, const HBITMAP hbitmap);

	const POINT GetNormalPos(const int index);
	void SetNormalPos(const int index, const POINT pos);

public:
	void NextSelectBitmapIndex();
};
