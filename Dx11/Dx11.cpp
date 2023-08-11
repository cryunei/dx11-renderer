﻿#include "Dx11.h"
#include "framework.h"
#include "Asset/CrAssetManager.h"
#include "Asset/CrVertexShader.h"
#include "Core/Dx11Device.h"
#include "Level/CrLevel.h"
#include "Render/Dx11Renderer.h"
#include "Fbx/FbxImportHelper.h"
#include "GUI/GuiManager.h"
#include "Loader/Dx11LevelLoader.h"


enum
{
    MAX_LOADSTRING = 100
};


// 전역 변수:
HINSTANCE hInst; // 현재 인스턴스입니다.

WCHAR szTitle[ MAX_LOADSTRING ];       // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[ MAX_LOADSTRING ]; // 기본 창 클래스 이름입니다.

Dx11Renderer G_Dx11Renderer;
Dx11LevelLoader G_Dx11LevelLoader;


// default functions
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
INT_PTR CALLBACK About( HWND, UINT, WPARAM, LPARAM );
void LoadAssets();


//=====================================================================================================================
// @brief	wWinMain
//=====================================================================================================================
int APIENTRY wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
    LoadStringW( hInstance, IDC_DX11, szWindowClass, MAX_LOADSTRING );
    MyRegisterClass( hInstance );

    // 애플리케이션 초기화를 수행합니다:
    if ( !InitInstance( hInstance, nCmdShow ) )
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_DX11 ) );

    MSG msg;

    // 기본 메시지 루프입니다:
    while ( TRUE )
    {
        if ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );

            if ( msg.message == WM_QUIT )
            {
                break;
            }
        }

        G_Dx11Renderer.RenderFrame();
    }

    return (int)msg.wParam;
}

//=====================================================================================================================
// @brief	MyRegisterClass
//=====================================================================================================================
ATOM MyRegisterClass( HINSTANCE hInstance )
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof( WNDCLASSEX );

    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_DX11 ) );
    wcex.hCursor       = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
    wcex.lpszMenuName  = MAKEINTRESOURCEW( IDC_DX11 );
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm       = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

    return RegisterClassExW( &wcex );
}

//=====================================================================================================================
// @brief	InitInstance
//=====================================================================================================================
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr );

    if ( !hWnd )
    {
        return FALSE;
    }

    G_Dx11Device.Create( hWnd );

    LoadAssets();

    G_Dx11Renderer.Initialize();
    GetGuiManager()->Initialize( hWnd );

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow( hWnd );

    return TRUE;
}

//=====================================================================================================================
// @brief	WndProc
//=====================================================================================================================
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if ( GetGuiManager()->WndProcHandler(hWnd, message, wParam, lParam) )
        return true;

    switch ( message )
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD( wParam );
            // 메뉴 선택을 구문 분석합니다:
            switch ( wmId )
            {
            case IDM_ABOUT: DialogBox( hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About );
                break;
            case IDM_EXIT: DestroyWindow( hWnd );

                G_Dx11Renderer.Clear();

                break;
            default: return DefWindowProc( hWnd, message, wParam, lParam );
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
        }
        break;
    case WM_DESTROY: PostQuitMessage( 0 );
        break;
    default: return DefWindowProc( hWnd, message, wParam, lParam );
    }
    return 0;
}

//=====================================================================================================================
// @brief	About
//=====================================================================================================================
INT_PTR CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    UNREFERENCED_PARAMETER( lParam );
    switch ( message )
    {
    case WM_INITDIALOG: return (INT_PTR)TRUE;

    case WM_COMMAND: if ( LOWORD( wParam ) == IDOK || LOWORD( wParam ) == IDCANCEL )
        {
            EndDialog( hDlg, LOWORD( wParam ) );
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

//=====================================================================================================================
// @brief   LoadAssets
//=====================================================================================================================
void LoadAssets()
{
    FbxImportHelper fbxImporter;
    fbxImporter.LoadAll( "../Asset/Fbx/wooden_sphere.fbx", "WoodenSphere" );
    fbxImporter.LoadAll( "../Asset/Fbx/Lowpoly_tree_sample.fbx", "Tree" );

    if ( CrVertexShader* vs = GetAssetManager()->CreateVertexShader( "DefaultDiffuse" ) )
    {
        vs->Initialize( "../Shader/Shader.hlsl", "VS", "vs_4_0",
        {
            { "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        } );
    }

    if ( CrPixelShader* ps = GetAssetManager()->CreatePixelShader( "DefaultDiffuse" ) )
    {
        ps->Initialize( "../Shader/Shader.hlsl", "PS", "ps_4_0" );
    }

    if ( CrPixelShader* ps = GetAssetManager()->CreatePixelShader( "Specular" ) )
    {
        ps->Initialize( "../Shader/Shader.hlsl", "PS_Specular", "ps_4_0" );
    }

    if ( CrPixelShader* ps = GetAssetManager()->CreatePixelShader( "PointLight" ) )
    {
        ps->Initialize( "../Shader/Shader.hlsl", "PS_PointLight", "ps_4_0" );
    }

    if ( CrPixelShader* ps = GetAssetManager()->CreatePixelShader( "HalfLambert" ) )
    {
        ps->Initialize( "../Shader/Shader.hlsl", "PS_HalfLambert", "ps_4_0" );
    }

    if ( CrPixelShader* ps = GetAssetManager()->CreatePixelShader( "Toon" ) )
    {
        ps->Initialize( "../Shader/Shader.hlsl", "PS_Toon", "ps_4_0" );
    }

    if ( CrTexture2D* tex = GetAssetManager()->CreateTexture2D( "BlockDiffuse" ) )
    {
        tex->SetPath  ( "../Asset/Texture/block.jpg" );
        tex->SetFormat( DXGI_FORMAT_R8G8B8A8_UNORM );
        tex->SetWidth ( 743 );
        tex->SetHeight( 484 );
        tex->SetSamplingCount( 1 );
    }

    if ( CrTexture2D* tex = GetAssetManager()->CreateTexture2D( "WoodenSphereDiffuse" ) )
    {
        tex->SetPath  ( "../Asset/Texture/mpm_vol.08_p16_light_side_A_diff.jpg" );
        tex->SetFormat( DXGI_FORMAT_R8G8B8A8_UNORM );
        tex->SetWidth ( 2000 );
        tex->SetHeight( 2000 );
        tex->SetSamplingCount( 1 );
    }

    CrLevel level;
    G_Dx11LevelLoader.Load( level );
    G_Dx11LevelLoader.AddRenderMeshes( G_Dx11Renderer );
}