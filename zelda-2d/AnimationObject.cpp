#include "pch.h"
#include "AnimationObject.h"

const HBITMAP AnimationObject::GetAnimationBitmap(const int index)
{
	if (index > static_cast<int>(bitmapData.size()) && 0 > index)
		return nullptr;

	return bitmapData[index];
}

void AnimationObject::SetBitmap(const int index, const HBITMAP hbitmap)
{
	if (index > static_cast<int>(bitmapData.size()) && 0 > index)
		return;

	bitmapData[index] = hbitmap;
}

const POINT AnimationObject::GetNormalPos(const int index)
{
	if (index > static_cast<int>(normalPos.size()) && 0 > index)
		return { NULL, NULL };

	return normalPos[index];
}

void AnimationObject::SetNormalPos(const int index, const POINT pos)
{
	if (index > static_cast<int>(normalPos.size()) && 0 > index)
		return;

	normalPos[index] = pos;
}