// Tetris.cpp : Defines the entry point for the application.
//

// 멀티바이트 유니코드 전처리
#if defined(UNICODE) || defined(_UNICODE)
    #pragma comment( linker, "/entry:wWinMainCRTStartup /subsystem:console" ) // 콘솔을 하위 시스템으로 설정
    #define tcout std::wcout
#else
    #pragma comment( linker, "/entry:WinMainCRTStartup /subsystem:console" )
    #define tcout std::cout
#endif

#include <fbxsdk.h>
#include <iostream>
#include <math.h>

#include "framework.h"
#include "MathUtils.h"
#include "Renderer.h"
#include "Tetris.h"

#include "Log.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
TCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

Renderer gRenderer;

// 로그 콜백 함수
void LogWrited(const TCHAR* log);

vector<Vertex>  gMesh;
vector<int>     gIndices;
int             gIndexCount = 0;

void Init()
{
    gRenderer.Initialize(GetActiveWindow());


    // FBX 로드
    FbxManager* manager = FbxManager::Create();

    FbxIOSettings* ioSettings = FbxIOSettings::Create(manager, IOSROOT);
    manager->SetIOSettings(ioSettings);

    FbxImporter* importer = FbxImporter::Create(manager, "");
    if (!importer->Initialize("box.fbx", -1, manager->GetIOSettings()))
    {
		LOG_CALL(_T("FbxImporter Initialize failed"));

        importer->Destroy();
		manager->Destroy();
		return;
	}
    else
    {
        FbxScene* scene = FbxScene::Create(manager, "tempName");

        importer->Import(scene);

        FbxNode* rootNode = scene->GetRootNode();
        if (rootNode)
        {
            int numChild = rootNode->GetChildCount();
            FbxNode* childNode = nullptr;

            for (int i = 0; i < numChild; ++i)
            {
                childNode = rootNode->GetChild(i);
                FbxMesh* mesh = childNode->GetMesh();

                if (mesh != nullptr)
                {
                    //Get Vertices
                    int numVertices = mesh->GetControlPointsCount();

                    for (int j = 0; j < numVertices; ++j)
                    {
                        gMesh.push_back(Vertex());

                        FbxVector4 vertex = mesh->GetControlPointAt(j);

                        gMesh[j].x = (float)vertex.mData[0];
                        gMesh[j].y = (float)vertex.mData[1];
                        gMesh[j].z = (float)vertex.mData[2];

                        LOG(_T("vertex[%d] : {%f, %f, %f}\n"), j, gMesh[j].x, gMesh[j].y, gMesh[j].z);
                    }
                }

                gIndexCount = mesh->GetPolygonVertexCount();
                int* indices = mesh->GetPolygonVertices();
                gIndices.insert(gIndices.begin(), &indices[0],  &indices[gIndexCount]);
                LOG(_T("Index count : %d\n"), gIndexCount);
            }
        }

        gRenderer.PushVertexBuffer(gMesh.data(), gIndices.data(), gIndexCount);

        scene->Destroy();
        importer->Destroy();
        manager->Destroy();
    }
}

void Update()
{
    /*
        Rotation
    */
    static float rX = 0.0f, rY = 0.0f, rZ = 0.0f;

    rX += 1.0f;
    float rotateRadian = rX * 3.141592f / 180;
    
    Matrix4 ZRotMatrix;
    Matrix4::Identity(ZRotMatrix);

    ZRotMatrix._m11 = cos(rotateRadian);
    ZRotMatrix._m12 = -sin(rotateRadian);
    ZRotMatrix._m21 = sin(rotateRadian);
    ZRotMatrix._m22 = cos(rotateRadian);

    Vector4 testVector = {1, 1, 1, 0};

    for (auto& vertex : gMesh)
    {
		Vector4 rotatedVector = ZRotMatrix * Vector4{vertex.x, vertex.y, vertex.z, 1};
		vertex.x = rotatedVector.x;
		vertex.y = rotatedVector.y;
		vertex.z = rotatedVector.z;
	}

    LOG(_T("Current Angle : {%f}\n"), rX);
}

void Render()
{
    RedrawWindow(GetActiveWindow(), 0, 0, RDW_INTERNALPAINT);
}

void Release()
{
    gRenderer.Release();
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    setlocale(LC_ALL, ".UTF8");                     // Unicode 한글 출력을 위한 로케일 설정, Windows 10 버전 1803(10.0.17134.0)부터만 UTF-8 지원
    AllocConsole();                                 // 콘솔창 띄우기
    Logger::GetInstance()->AddCallback(LogWrited);  // 로그 콜백 함수 등록

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_TETRIS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    // inialize renderer
    Init();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRIS));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        Update();
        Render();
    }

    Release();

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRIS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_TETRIS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_ACTION_DRAW:
                LOG(_T("%s\n"), _T("Draw 버튼 클릭됨"));
                Render();
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                LOG(_T("%s\n"), _T("DialogBox 버튼 클릭됨"));
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            {
                // TODO: Add any drawing code that uses hdc here...
                InvalidateRect(hWnd,NULL,TRUE);
                gRenderer.OnPaint(hdc);
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

void LogWrited(const TCHAR* log)
{
    tcout << log;
}