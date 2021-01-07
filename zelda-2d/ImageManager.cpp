#include "pch.h"
#include "ImageManager.h"

extern HINSTANCE hInst;

ImageManager* ImageManager::instance = nullptr;
ImageManager::ImageManager()
{
	mainFrameBitmap = (HBITMAP)LoadImageA(hInst, "images/bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

ImageManager::~ImageManager()
{
}

ImageManager* ImageManager::GetInstance()
{
	if (nullptr == instance)
		instance = new ImageManager();

	return instance;
}

void ImageManager::ReleaseInstance()
{
	delete instance;
	instance = nullptr;
}

void ImageManager::LoadBitmapPathData(const MapEdittorSelectState state, const string str)
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

			switch (state)
			{
			case MapEdittorSelectState::BACKGROUND:
				backgroundBitmapData[stoi(number)] = (HBITMAP)LoadImageA(hInst, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				break;
			default:
				objectBitmapData[stoi(number)] = (HBITMAP)LoadImageA(hInst, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				break;
			}
		}
	}

	readFile.close();
}

void ImageManager::LoadMapEdittorBitmap()
{
	LoadBitmapPathData(MapEdittorSelectState::BACKGROUND, "data/BackgroundBitmapInfo.txt");
	LoadBitmapPathData(MapEdittorSelectState::OBJECT, "data/ObjectBitmapInfo.txt");
}

const HBITMAP ImageManager::GetMainFrameBitmap()
{
	return mainFrameBitmap;
}

const HBITMAP ImageManager::GetBitmap(MapEdittorSelectState state, int selectNumber)
{
	switch (state)
	{
	case MapEdittorSelectState::BACKGROUND:
		return backgroundBitmapData[selectNumber];
	case MapEdittorSelectState::OBJECT:
		return objectBitmapData[selectNumber];
	default:
		break;
	}
}

const map<int, HBITMAP> ImageManager::GetBitmapMapVar(const BitmapKind kind)
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

