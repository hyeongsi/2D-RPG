#include "pch.h"
#include "ImageManager.h"

extern HINSTANCE hInst;

void ImageManager::LoadBitmapPathData(const string str)
{
	ifstream readFile;

	readFile.open(str);
	if (readFile.is_open())
	{
		while (!readFile.eof())
		{
			string number;
			readFile >> number;

			string path;
			readFile >> path;

			backgroundBitmapData[stoi(number)] = (HBITMAP)LoadImageA(hInst, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}
	}

	readFile.close();
}

void ImageManager::LoadMapEdittorBitmap()
{
	LoadBitmapPathData("data/BackgroundBitmapInfo.txt");
	LoadBitmapPathData("data/ObjectBitmapInfo.txt");
}

const map<int, HBITMAP> ImageManager::GetBitmapData(const BitmapKind kind)
{
	switch (kind)
	{
	case BitmapKind::BACKGROUND:
		return backgroundBitmapData;
	case BitmapKind::OBJECT:
		return objectBitmapData;
	default:
		return backgroundBitmapData;
	}
}

