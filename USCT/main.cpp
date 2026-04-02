#include <windows.h>
#include <d3d11.h>
#include <tchar.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "gui.h"
#include "serverconfig.h"
#include "setting.h"
#include "ModConfig.h"

#pragma comment(lib, "d3d11.lib")

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

LRESULT WINAPI WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
    if (ImGui_ImplWin32_WndProcHandler(hWnd,msg,wParam,lParam))
        return true;

    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd,msg,wParam,lParam);
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0,IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer,nullptr,&g_mainRenderTargetView);
    pBackBuffer->Release();
}

bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    sd.BufferUsage =
        DXGI_USAGE_RENDER_TARGET_OUTPUT;

    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;

    return SUCCEEDED(D3D11CreateDeviceAndSwapChain(nullptr,D3D_DRIVER_TYPE_HARDWARE,nullptr,0,nullptr,0,D3D11_SDK_VERSION,&sd,&g_pSwapChain,&g_pd3dDevice,nullptr,&g_pd3dDeviceContext)
    );
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,int)
{
    WNDCLASSEX wc = {sizeof(WNDCLASSEX),CS_CLASSDC,WndProc,0,0,hInstance,nullptr,nullptr,nullptr,nullptr,_T("ImGuiApp"),nullptr};
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(101));
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(101));
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName,_T("Unturned Server Config Tool"),WS_OVERLAPPED |WS_CAPTION |WS_SYSMENU |WS_MINIMIZEBOX,100,100,534,500,nullptr,nullptr,wc.hInstance,nullptr);

    CreateDeviceD3D(hwnd);
    CreateRenderTarget();
    sc_PVP = true;
    LoadServerConfig();
    LoadFileIDs();
    
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice,g_pd3dDeviceContext);

    gui::init();

    MSG msg = {};

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg,nullptr,0,0,PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();
        gui::render();
        ImGui::Render();

        const float clear_color[4] = {0.1f,0.1f,0.1f,1.0f};

        g_pd3dDeviceContext->OMSetRenderTargets(1,&g_mainRenderTargetView,nullptr);

        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView,clear_color);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1,0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();

    return 0;
}