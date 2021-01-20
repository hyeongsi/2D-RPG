// zelda-2d.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "zelda-2d.h"
#include "GameManager.h"
#include "mapEdittor.h"
#include "imageManager.h"
#include "renderManager.h"
#include "Player.h"
#include <commdlg.h>
#include "InteractionManager.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND g_hWnd;                                    // 전역 윈도우 핸들
HWND g_hMapEdittorDlg;                          // 전역 맵에디터 다이얼로그 핸들
HWND g_hInventoryDlg;                           // 전역 인벤토리 다이얼로그 핸들
RECT g_clientRect{ 0,0, ClientSize::width,ClientSize::height }; // 클라이언트 크기
SIZE g_clientSize;                              // 클라이언트 사이즈

HWND g_hStartButton;                            // 시작 버튼
HWND g_hMapEdittorButton;                       // 맵 에디터 버튼

ClickLR clickLR{ ClickLR::NONE };

GameManager* gameManager;                       // 게임 매니저
MapEdittor* mapEdittor;                         // 맵 에디터
ImageManager* imageManager;                     // 이미지 매니저
RenderManager* renderManager;                   // 랜더 매니저

Player* character;                            // 캐릭터 클래스

void SetMapEdittorData();                       // 함수 선언
void SaveTextMapData(const char* filePath);           // 맵 정보 저장
void SetMapEdittorDlgData();                                // mapEdittorDlg 데이터 설정
void GetSelectListBoxData(const MapEdittorSelectState state);     // mapEdittorDlg 리스트박스 데이터 가져오기
void SelectListBoxSetting(const MapEdittorSelectState state);     // mapEdittorDlg 리스트박스, 버튼 선택 시 설정
void ShowMainFrameButton();                                 // 버튼 출력
void HideMainFrameButton();                                 // 버튼 숨기기

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    MapEdittorDlg(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    InventoryDlg(HWND, UINT, WPARAM, LPARAM);

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

        switch (gameManager->GetState())
        {
        case GameState::MAIN:
            renderManager->MainFrameDataRender();
            break;
        case GameState::MAPEDITTOR:
            renderManager->MapEdittorDataRender();
            break;
        case GameState::INGAME:
            gameManager->Input();
            gameManager->Run();
            renderManager->InGameDataRender(
                gameManager->GetPlayer(), gameManager->GetNPC(), 
                gameManager->GetFieldItem(gameManager->GetCurrentStage()));
            break;
        default:
            break;
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
        MoveWindow(hWnd, HWND_SPAWN_POS.x, HWND_SPAWN_POS.y, g_clientSize.cx, g_clientSize.cy, true);   // 500,200 지점에 클라이언트 크기만큼 설정 후 출력
        
        g_hStartButton = CreateWindow("button", "Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
            , START_BUTTON_POINT.x, START_BUTTON_POINT.y, BUTTON_SIZE.cx, BUTTON_SIZE.cy, hWnd, (HMENU)ButtonKind::START, hInst, NULL);    // 메인화면의 시작 버튼 생성

        g_hMapEdittorButton = CreateWindow("button", "MapEdittor", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
            , MAPEDITTOR_BUTTON_POINT.x, MAPEDITTOR_BUTTON_POINT.y, BUTTON_SIZE.cx, BUTTON_SIZE.cy, hWnd, (HMENU)ButtonKind::MAPEDITTOR, hInst, NULL);    // 메인 화면의 맵 에디터 버튼 생성
        
        break;
    case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
            case IDC_lNVEN_OPEN:
                ShowWindow(g_hInventoryDlg, SW_SHOW);
                break;
            case IDC_lNVEN_CLOSE:
                ShowWindow(g_hInventoryDlg, SW_HIDE);
                break;
            case ButtonKind::START:         // 시작 버튼 누르면
                HideMainFrameButton();                               // 버튼 숨기기
                gameManager->SetState(GameState::INGAME);            // 인게임 실행
                gameManager->SetPlayer(new Player());           // 플레이어 생성
                gameManager->SetInventory(new Inventory());     // 인벤토리 생성
                gameManager->SetNPC(new NPC());                 // npc 생성

                imageManager->LoadMapBitmapData();                      // 인게임에서 사용할 맵 관련 비트맵 로드
                imageManager->LoadAnimationBitmapData(PLAYER_ANIMATION_PATH);    // 인게임에서 사용할 플레이어 애니메이션 로드
                imageManager->LoadAnimationBitmapData(NPC_ANIMATION_PATH);    // 인게임에서 사용할 NPC 애니메이션 로드
                imageManager->LoadAnimationBitmapData(HEART_ANIMATION_PATH);    // 인게임에서 하트 UI 애니메이션 로드
                imageManager->LoadAnimationBitmapData(MONEY_ANIMATION_PATH);    // 인게임에서 돈 UI 애니메이션 로드
                imageManager->LoadBitmapPathData(BitmapKind::UI, UI_BITMAP_PATH);   // 인게임에서 사용할 UI 비트맵 로드
                InteractionManager::GetInstance()->LoadTextMapData(GameState::INGAME, STAGE1_PATH); // 인게임에서 사용할 맵데이터 로드
                InteractionManager::GetInstance()->LoadTextEventData(STAGE1_EVENT_PATH, gameManager->GetWorldMapData(gameManager->GetCurrentStage()));
                
                g_hInventoryDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_INVENTORY), hWnd, InventoryDlg);  // 다이얼로그 생성
                RECT invenDlgRect;
                SIZE invenDlgSize;
                GetWindowRect(g_hInventoryDlg, &invenDlgRect);
                invenDlgSize.cx = invenDlgRect.right - invenDlgRect.left;
                invenDlgSize.cy = invenDlgRect.bottom - invenDlgRect.top;
                MoveWindow(g_hInventoryDlg, HWND_SPAWN_POS.x + 530, HWND_SPAWN_POS.y + 50,
                    invenDlgSize.cx, invenDlgSize.cy, true);   // 해당 지점에 클라이언트 크기만큼 설정 후 출력
                // 인벤토리 창 생성
                break;
            case ButtonKind::MAPEDITTOR:       // 맵에디터 버튼 누르면
                HideMainFrameButton();                               // 버튼 숨기기

                gameManager->SetState(GameState::MAPEDITTOR);        // 맵 에디터 실행
                imageManager->LoadMapBitmapData();                // 맵 에디터에서 사용할 이미지 로드
                mapEdittor->Init();

                g_hMapEdittorDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, MapEdittorDlg);  // 다이얼로그 생성
                SetMapEdittorDlgData();

                RECT dlgRect;
                SIZE dlgSize;
                GetWindowRect(g_hMapEdittorDlg, &dlgRect);
                dlgSize.cx = dlgRect.right - dlgRect.left;
                dlgSize.cy = dlgRect.bottom - dlgRect.top;
                MoveWindow(g_hMapEdittorDlg, 500 + g_clientSize.cx, 200,
                    dlgSize.cx, dlgSize.cy, true);   // 해당 지점에 클라이언트 크기만큼 설정 후 출력

                ShowWindow(g_hMapEdittorDlg, SW_SHOW);

                InvalidateRect(hWnd, nullptr, true);    // 화면 초기화
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
        if(0x00000000 != g_hInventoryDlg)
            DestroyWindow(g_hInventoryDlg);                    // 다이얼로그 삭제

        GameManager::ReleaseInstance();
        MapEdittor::ReleaseInstance();
        ImageManager::ReleaseInstance();
        RenderManager::ReleaseInstance();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK MapEdittorDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    OPENFILENAME openFileName;
    static char strFileTitle[MAX_PATH], strFileExtension[10], strFilePath[100];
    TCHAR curDirectoryPath[256];

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_lBACKGROUND:
        case IDC_rBACKGROUND:
            SelectListBoxSetting(MapEdittorSelectState::BACKGROUND);
            return (INT_PTR)TRUE;
        case IDC_lOBJECT:
        case IDC_rOBJECT:
            SelectListBoxSetting(MapEdittorSelectState::OBJECT);
            return (INT_PTR)TRUE;
        case IDC_rCOLLIDER:
            mapEdittor->SetSelectState(MapEdittorSelectState::COLLIDER);   
            return (INT_PTR)TRUE;
        case IDC_bSAVE: 
            GetCurrentDirectory(256, curDirectoryPath);         // GetOpenFileName 호출하면 기본 경로명이 바뀌기 때문에 기본 경로명 미리 저장

            ZeroMemory(&openFileName, sizeof(OPENFILENAME));    // 구조체를 0으로 셋업
            openFileName.lStructSize = sizeof(OPENFILENAME);
            openFileName.hwndOwner = g_hWnd;
            openFileName.lpstrTitle = "저장";
            openFileName.lpstrFileTitle = strFileTitle;
            openFileName.lpstrFile = strFilePath;
            openFileName.lpstrFilter = "맵 데이터(*.txt)\0*.txt\0";
            openFileName.nMaxFile = MAX_PATH;
            openFileName.nMaxFileTitle = MAX_PATH;

            if (GetSaveFileName(&openFileName) != 0)    // 인덱스가 1부터 시작하기 때문에 지정
            {
                switch (openFileName.nFilterIndex)
                {
                case 1:
                    SaveTextMapData(strFilePath);
                    break;
                default:
                    break;
                }
            }
            SetCurrentDirectory(curDirectoryPath);  // 변경된 경로를 원래 경로로 설정
            return (INT_PTR)TRUE;
        case IDC_bLOAD:
            GetCurrentDirectory(256, curDirectoryPath);         // GetOpenFileName 호출하면 기본 경로명이 바뀌기 때문에 기본 경로명 미리 저장

            ZeroMemory(&openFileName, sizeof(openFileName));    // 구조체를 0으로 셋업
            openFileName.lStructSize = sizeof(openFileName);
            openFileName.hwndOwner = g_hWnd;
            openFileName.lpstrTitle = "로드";
            openFileName.lpstrFileTitle = strFileTitle;
            openFileName.lpstrFile = strFilePath;
            openFileName.lpstrFilter = "맵 데이터(*.txt)\0*.txt\0";
            openFileName.nMaxFile = MAX_PATH;
            openFileName.nMaxFileTitle = MAX_PATH;        

            if (GetOpenFileName(&openFileName) != 0)    // 인덱스가 1부터 시작하기 때문에 지정
            {
                switch (openFileName.nFilterIndex)
                {
                case 1:
                    InteractionManager::GetInstance()->LoadTextMapData(GameState::MAPEDITTOR, strFilePath);
                    break;
                default:
                    break;
                }
            }
            SetCurrentDirectory(curDirectoryPath);  // 변경된 경로를 원래 경로로 설정

            return (INT_PTR)TRUE;
        case IDC_bEXIT:
            ShowMainFrameButton();

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

INT_PTR CALLBACK InventoryDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_SYSCOMMAND:
        switch (LOWORD(wParam))
        {
        case SC_CLOSE:
            ShowWindow(g_hInventoryDlg, SW_HIDE);
            return (INT_PTR)TRUE;
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_bSAVE:
            return (INT_PTR)TRUE;
        case IDC_bLOAD:
            return (INT_PTR)TRUE;
        default:
            break;
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

void SaveTextMapData(const char* filePath)
{
    ofstream writeFile;
    int value[2] = { MAP_MAX_Y , MAP_MAX_X };   // 상단 맵 크기 저장할 변수
    WorldMap mapData = mapEdittor->GetWorldMapData();   // 저장할 맵 에디터의 맵 데이터
    MapEdittorSelectState selectState;
    string str;
    try
    {
        writeFile.open(filePath);
        if (writeFile.is_open())
        {
       
            for (int i = 0; i < 2; i++) // 맵의 크기, x,y값 저장
            {
                writeFile << value[i]<<' ';
            }

            writeFile << '\n';

            for (int i = 0; i < 4; i++) // 배경, 오브젝트, 콜라이더 데이터를 받는다.
            {
                switch (i)
                {
                case 0:
                    selectState = MapEdittorSelectState::BACKGROUND;
                    str = "background";                         break;
                case 1:
                    selectState = MapEdittorSelectState::OBJECT;
                    str = "objects";                            break;
                case 2:
                    selectState = MapEdittorSelectState::COLLIDER;
                    str = "collider";                           break;
                }
                writeFile << str<<'\n';    // background, objects, colider 구분 문자열

                for (int y = 0; y < MAP_MAX_Y; y++)
                {
                    for (int x = 0; x < MAP_MAX_X; x++)
                    {
                        writeFile << mapData.GetData(selectState, { x,y }) << ' ';
                    }
                    writeFile << '\n';
                }
            }
        }
        
    }
    catch (const std::exception&)
    {
        
    }

    writeFile.close();
}

void SetMapEdittorDlgData()
{
    SendMessage(GetDlgItem(g_hMapEdittorDlg, IDC_rBACKGROUND), BM_SETCHECK, BST_CHECKED, 0);    // 버튼 선택상태 초기화

    HWND hwndBackGroundImageListBox = GetDlgItem(g_hMapEdittorDlg, IDC_lBACKGROUND);
    int i = 1;
    while (true)    // 리스트 박스 배경 문자열 추가 코드
    {
        string name = ImageManager::GetInstance()->GetStringData(BitmapKind::BACKGROUND, i++);
        if (name.empty())
            break;
        SendMessage(hwndBackGroundImageListBox, LB_ADDSTRING, 0, (LPARAM)name.c_str());
    }

    HWND hwndObjectImageListBox = GetDlgItem(g_hMapEdittorDlg, IDC_lOBJECT);

    i = 1;
    while (true)    // 리스트 박스 오브젝트 문자열 추가 코드
    {
        string name = ImageManager::GetInstance()->GetStringData(BitmapKind::OBJECT, i++);
        if (name.empty())
            break;
        SendMessage(hwndObjectImageListBox, LB_ADDSTRING, 0, (LPARAM)name.c_str());
    }

    SendMessage(GetDlgItem(g_hMapEdittorDlg, IDC_lBACKGROUND), LB_SETCURSEL, 0, 0);     // 리스트박스 선택 초기화
    SendMessage(GetDlgItem(g_hMapEdittorDlg, IDC_lOBJECT), LB_SETCURSEL, 0, 0);         // 리스트박스 선택 초기화
}

void GetSelectListBoxData(const MapEdittorSelectState state)
{
    HWND hwndList;

    switch (state)
    {
    case MapEdittorSelectState::BACKGROUND:
        hwndList = GetDlgItem(g_hMapEdittorDlg, IDC_lBACKGROUND);       break;
    case MapEdittorSelectState::OBJECT:
        hwndList = GetDlgItem(g_hMapEdittorDlg, IDC_lOBJECT);           break;
    default :
        return;
    }

    int selectListBoxItemIndex = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);

    selectListBoxItemIndex += 1;        // 이미지 인덱스 1부터 시작하기 때문에 +1 시켜줌
    MapEdittor::GetInstance()->SetSelectIndex(selectListBoxItemIndex);
}

void SelectListBoxSetting(const MapEdittorSelectState state)
{
    int unCheckButton;
    int unCheckButton2;
    int checkButton;

    switch (state)
    {
    case MapEdittorSelectState::BACKGROUND:
        unCheckButton = IDC_rOBJECT;
        unCheckButton2 = IDC_rCOLLIDER;
        checkButton = IDC_rBACKGROUND;
        break;
    case MapEdittorSelectState::OBJECT:
        unCheckButton = IDC_rBACKGROUND;
        unCheckButton2 = IDC_rCOLLIDER;
        checkButton = IDC_rOBJECT; 
        break;
    default:
        return;
    }

    SendMessage(GetDlgItem(g_hMapEdittorDlg, unCheckButton), BM_SETCHECK, BST_UNCHECKED, 0);        // 버튼 선택상태 초기화
    SendMessage(GetDlgItem(g_hMapEdittorDlg, unCheckButton2), BM_SETCHECK, BST_UNCHECKED, 0);        // 버튼 선택상태 초기화
    SendMessage(GetDlgItem(g_hMapEdittorDlg, checkButton), BM_SETCHECK, BST_CHECKED, 0);            // 버튼 선택상태 초기화
    mapEdittor->SetSelectState(state);
    GetSelectListBoxData(state);
}

void ShowMainFrameButton()
{
    ShowWindow(g_hStartButton, SW_SHOW);                // 버튼 출력
    ShowWindow(g_hMapEdittorButton, SW_SHOW);           // 버튼 출력
}

void HideMainFrameButton()
{
    ShowWindow(g_hStartButton, SW_HIDE);                // 버튼 숨기기
    ShowWindow(g_hMapEdittorButton, SW_HIDE);           // 버튼 숨기기
}