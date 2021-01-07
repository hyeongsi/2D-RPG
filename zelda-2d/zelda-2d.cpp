// zelda-2d.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "zelda-2d.h"
#include "GameManager.h"
#include "mapEdittor.h"
#include "imageManager.h"
#include "renderManager.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND g_hWnd;                                    // 전역 윈도우 핸들
HWND g_hMapEdittorDlg;                          // 전역 맵에디터 다이얼로그 핸들
RECT g_clientRect{ 0,0, ClientSize::width,ClientSize::height }; // 클라이언트 크기
SIZE g_clientSize;                              // 클라이언트 사이즈

HWND g_hStartButton;                            // 시작 버튼
HWND g_hMapEdittorButton;                       // 맵 에디터 버튼

ClickLR clickLR{ ClickLR::NONE };

GameManager* gameManager;                        // 게임 매니저
MapEdittor* mapEdittor;                         // 맵 에디터
ImageManager* imageManager;                      // 이미지 매니저
RenderManager* renderManager;                    // 랜더 매니저

void SetMapEdittorData();                       // 함수 선언

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    MapEdittorDlg(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ZELDA2D, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ZELDA2D));

    MSG msg;

    // 초기화
    gameManager = GameManager::GetInstance();
    mapEdittor = MapEdittor::GetInstance();  
    imageManager = ImageManager::GetInstance();
    renderManager = RenderManager::GetInstance();

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg,nullptr,0,0,PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        if (GameState::MAPEDITTOR != gameManager->GetState())
        {
            renderManager->RenderInitSetting();
            gameManager->Run();
            renderManager->MainFrameDataSetting();
            renderManager->Render();
        }
        else
        {
            renderManager->RenderInitSetting();
            renderManager->MapEdittorDataSetting();
            renderManager->Render();
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ZELDA2D));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL; // MAKEINTRESOURCEW(IDC_ZELDA2D);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   g_hWnd = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static LPDRAWITEMSTRUCT lpdis;

    switch (message)
    {
    case WM_CREATE:
        AdjustWindowRect(&g_clientRect, WS_OVERLAPPEDWINDOW, false);    // 메뉴창 크기 빼고 윈도우 크기 계산
        g_clientSize.cx = g_clientRect.right - g_clientRect.left;
        g_clientSize.cy = g_clientRect.bottom - g_clientRect.top;
        MoveWindow(hWnd, 500, 200, g_clientSize.cx, g_clientSize.cy, true);   // 500,200 지점에 클라이언트 크기만큼 설정 후 출력
        
        g_hStartButton = CreateWindow("button", "Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
            , 500, 100, 150, 50, hWnd, (HMENU)ButtonKind::START, hInst, NULL);    // 메인화면의 시작 버튼 생성

        g_hMapEdittorButton = CreateWindow("button", "MapEdittor", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
            , 500, 200, 150, 50, hWnd, (HMENU)ButtonKind::MAPEDITTOR, hInst, NULL);    // 메인 화면의 맵 에디터 버튼 생성
        
        break;
    case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
            case ButtonKind::START:

                break;
            case ButtonKind::MAPEDITTOR:
                if (GameState::MAPEDITTOR != gameManager->GetState())    // 맵 에디터 상태가 아니면
                {
                    ShowWindow(g_hStartButton, SW_HIDE);                // 버튼 숨기기
                    ShowWindow(g_hMapEdittorButton, SW_HIDE);           // 버튼 숨기기

                    gameManager->SetState(GameState::MAPEDITTOR);        // 맵 에디터 실행

                    imageManager->LoadMapEdittorBitmap();                // 맵 에디터에서 사용할 이미지 로드

                    mapEdittor->Init();
                    g_hMapEdittorDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, MapEdittorDlg);  // 다이얼로그 생성

                    RECT dlgRect;
                    SIZE dlgSize;
                    GetWindowRect(g_hMapEdittorDlg, &dlgRect);
                    dlgSize.cx = dlgRect.right - dlgRect.left;
                    dlgSize.cy = dlgRect.bottom - dlgRect.top;
                    MoveWindow(g_hMapEdittorDlg, 500 + g_clientSize.cx, 200,
                        dlgSize.cx, dlgSize.cy, true);   // 해당 지점에 클라이언트 크기만큼 설정 후 출력

                    ShowWindow(g_hMapEdittorDlg, SW_SHOW);
                }

                InvalidateRect(hWnd, nullptr, true);    // 화면 초기화
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_LBUTTONDOWN:
        if (GameState::MAPEDITTOR == gameManager->GetState())    // 맵 에디터 상태
        {
            clickLR = ClickLR::LEFT;    // 클릭 상태 설정
            SetMapEdittorData();        // 맵에디터에 데이터 추가
        }
        break;
    case WM_RBUTTONDOWN:
        if (GameState::MAPEDITTOR == gameManager->GetState())    // 맵 에디터 상태
        {
            clickLR = ClickLR::RIGHT;   // 클릭 상태 설정
            SetMapEdittorData();        // 맵에디터에 데이터 추가
        }
        break;
    case WM_MOUSEMOVE:
        if (GameState::MAPEDITTOR == gameManager->GetState())    // 맵 에디터 상태
        {
            SetMapEdittorData();        // 맵에디터에 데이터 추가
        }
        break;
    case WM_LBUTTONUP:
        if (GameState::MAPEDITTOR == gameManager->GetState())    // 맵 에디터 상태
        {
            clickLR = ClickLR::NONE;    // 클릭 상태 설정
        } 
        break;
    case WM_RBUTTONUP:
        if (GameState::MAPEDITTOR == gameManager->GetState())
        {
            clickLR = ClickLR::NONE;    // 클릭 상태 설정
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        MapEdittor::ReleaseInstance();
        ImageManager::ReleaseInstance();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK MapEdittorDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_bEXIT:
            ShowWindow(g_hStartButton, SW_SHOW);                // 버튼 출력
            ShowWindow(g_hMapEdittorButton, SW_SHOW);           // 버튼 출력

            gameManager->SetState(GameState::MAIN);              // 상태값 변경
            DestroyWindow(g_hMapEdittorDlg);                    // 다이얼로그 삭제

            InvalidateRect(g_hWnd, nullptr, true);              // 화면 초기화
            return (INT_PTR)TRUE;
        default:
            break;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void SetMapEdittorData()
{
    if (GameState::MAPEDITTOR != gameManager->GetState())
        return;

    POINT mousePoint;
    GetCursorPos(&mousePoint);              // 커서 위치를 가져오고
    ScreenToClient(g_hWnd, &mousePoint);    // 클라이언트 영역 좌표로 변환 후

    switch (clickLR) // 왼쪽 오른쪽 마우스 판단 후 맵 데이터 저장
    {
    case ClickLR::NONE:
        break;
    case ClickLR::LEFT:
        mapEdittor->SetMapData(mousePoint, true);   // 맵에 선택된 이미지 정보 저장
        break;
    case ClickLR::RIGHT:
        mapEdittor->SetMapData(mousePoint, false);   // 맵에 선택된 이미지 정보 저장
        break;
    default:
        break;
    }
}