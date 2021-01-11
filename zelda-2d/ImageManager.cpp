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

void ImageManager::LoadBitmapPathData(const BitmapKind kind, const string str)
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

			string name;
			readFile >> name;

			switch (kind)
			{
			case BitmapKind::BACKGROUND:
				backgroundBitmapData[stoi(number)] = (HBITMAP)LoadImageA(hInst, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				backgroundStringData[stoi(number)] = name;
				break;
			case BitmapKind::OBJECT:
				objectBitmapData[stoi(number)] = (HBITMAP)LoadImageA(hInst, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				objectStringData[stoi(number)] = name;
				break;
			case BitmapKind::UI:
				uiBitmapData[stoi(number)] = (HBITMAP)LoadImageA(hInst, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				break;
			}
		}
	}

	readFile.close();
}

void ImageManager::LoadBitmapData()
{
	LoadBitmapPathData(BitmapKind::BACKGROUND, "data/bitmapifo/BackgroundBitmapInfo.txt");
	LoadBitmapPathData(BitmapKind::OBJECT, "data/bitmapifo/ObjectBitmapInfo.txt");
}

const HBITMAP ImageManager::GetMainFrameBitmap()
{
	return mainFrameBitmap;
}

const HBITMAP ImageManager::GetBitmapData(const BitmapKind kind,const int selectNumber)
{
	try
	{
		switch (kind)
		{
		case BitmapKind::BACKGROUND:
			return backgroundBitmapData[selectNumber];
		case BitmapKind::OBJECT:
			return objectBitmapData[selectNumber];
		}
	}
	catch (const std::exception&)
	{
		return NULL;
	}
}

const string ImageManager::GetStringData(const BitmapKind kind, const int selectNumber)
{
	try
	{
		switch (kind)
		{
		case BitmapKind::BACKGROUND:
			return backgroundStringData[selectNumber];
		case BitmapKind::OBJECT:
			return objectStringData[selectNumber];
		}
	}
	catch (const std::exception&)
	{
		return NULL;
	}
}