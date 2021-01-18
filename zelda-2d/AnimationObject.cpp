#include "pch.h"

#include "AnimationObject.h"

void AnimationObject::AddAnimationBitmap(const HBITMAP hbitmap)
{
	animationBitmapData.emplace_back(hbitmap);
}

void AnimationObject::AddBitmapCount(const int count)
{
	bitmapCount.emplace_back(count);
}

void AnimationObject::AddDelay(const int delay)
{
	this->delay.emplace_back(delay);
}

const int AnimationObject::GetAnimationBitmapSize()
{
	return animationBitmapData.size();
}

const int AnimationObject::GetBitmapCount(const int index)
{
	if (0 <= index && index < static_cast<int>(bitmapCount.size()))
		return bitmapCount[index];
	else
		return NULL;
}

const int AnimationObject::GetSelectAnimationBitmapIndex()
{
	return selectAnimationBitmapIndex;
}

void AnimationObject::SetSelectAnimationBitmapIndex(const int index)
{
	if (index < static_cast<int>(animationBitmapData.size()) && 0 <= index)
		selectAnimationBitmapIndex = index;
}

const int AnimationObject::GetSelectBitmapIndex()
{
	return selectBitmapIndex;
}

void AnimationObject::SetSelectBitmapIndex(const int index)
{
	selectBitmapIndex = index;
}

const HBITMAP AnimationObject::GetAnimationBitmap(const int index)
{
	if (index < static_cast<int>(animationBitmapData.size()) && 0 <= index)
		return animationBitmapData[index];
	else
		return nullptr;

}

void AnimationObject::SetBitmap(const int index, const HBITMAP hbitmap)
{
	if (index < static_cast<int>(animationBitmapData.size()) && 0 <= index)
		animationBitmapData[index] = hbitmap;
}

bool AnimationObject::NextSelectBitmapIndex()
{
	if (GetTickCount64() > tick + (delay[selectAnimationBitmapIndex]))
	{
		tick = GetTickCount64();
		++selectBitmapIndex;
	}	

	if (selectBitmapIndex >= bitmapCount[selectAnimationBitmapIndex])
	{
		selectBitmapIndex = 0;
		return true;
	}

	return false;
}
