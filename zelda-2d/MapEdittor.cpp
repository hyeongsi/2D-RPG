#include "pch.h"
#include "MapEdittor.h"

extern HWND g_hWnd;
extern SIZE g_clientSize;
extern HINSTANCE hInst;

MapEdittor::MapEdittor()
{
    Init();
}

MapEdittor::~MapEdittor()
{
    DeleteDC(memDC);
    DeleteDC(backMemDC);
    ReleaseDC(g_hWnd, hdc);
}

void MapEdittor::Init()
{
    if (hdc != nullptr)
    {
        ReleaseDC(g_hWnd, hdc);
        hdc = nullptr;
    }
    if (memDC != nullptr)
    {
        DeleteObject(memDC);
        memDC = nullptr;
    }

    if (backMemDC != nullptr)
    {
        DeleteObject(backMemDC);
        backMemDC = nullptr;
    }

    hdc = GetDC(g_hWnd);
    memDC = CreateCompatibleDC(hdc);
    backMemDC = CreateCompatibleDC(hdc);

    selectBitmap = BackGroundTextureName::grass;
    selectState = MapEdittorSelectState::BACKGROUND;
    hBitmap = (HBITMAP)LoadImageA(hInst, "images/backgrounds/grass.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

void MapEdittor::Run()
{
	Render();
}

void MapEdittor::Render()
{
    if (oldCreateBitmap != nullptr)
    {
        DeleteObject(oldCreateBitmap);
        oldCreateBitmap = nullptr;
    }

	oldCreateBitmap = (HBITMAP)SelectObject(memDC, CreateCompatibleBitmap(hdc, ClientSize::width, ClientSize::height));
	RECT windowRect{ 0,0,ClientSize::width,ClientSize::height };
	FillRect(memDC, &windowRect, (HBRUSH)GetStockObject(WHITE_BRUSH));      // 바탕 흰색으로 초기화

    POINT mousePoint;       // 마우스 위치를 저장할 변수

    // 세로선
    for (int i = 0; i < g_clientSize.cx / TILE_SIZE; i++)
    {
        MoveToEx(memDC, i * TILE_SIZE, 0, NULL);
        LineTo(memDC, i * TILE_SIZE, g_clientSize.cy);
    }

    // 가로선
    for (int i = 0; i < g_clientSize.cy / TILE_SIZE; i++)
    {
        MoveToEx(memDC, 0, i * TILE_SIZE, NULL);
        LineTo(memDC, g_clientSize.cx, i * TILE_SIZE);
    }

    // 마우스 위치에 비트맵 출력
    GetCursorPos(&mousePoint);              // 커서 위치를 가져오고
    ScreenToClient(g_hWnd, &mousePoint);    // 클라이언트 영역 좌표로 변환 후

    HBITMAP oldBitmap = (HBITMAP)SelectObject(backMemDC, hBitmap);

    BitBlt(memDC, mousePoint.x, mousePoint.y, TILE_SIZE, TILE_SIZE, backMemDC, 0, 0, SRCCOPY);
    BitBlt(hdc, 0, 0, ClientSize::width, ClientSize::height, memDC, 0, 0, SRCCOPY);
}
