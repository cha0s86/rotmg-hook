#define WIN32_LEAN_AND_MEAN
#define IMGUI_DEFINE_MATH_OPERATORS
#include <Windows.h>
#include <iostream>
#include <d3d11.h>
#include "cha0s.h"
#include "include/imgui/imgui.h"
#include "include/imgui/imgui_impl_dx11.h"
#include "include/imgui/imgui_impl_win32.h"
using namespace std;

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;  

Present oPresent;
inline HWND window = NULL;
inline WNDPROC oWndProc;
inline ID3D11Device* pDevice = nullptr;
inline ID3D11DeviceContext* pDeviceContext = nullptr;
inline ID3D11RenderTargetView* mainRenderTargetView = nullptr;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool onoroff = false;

bool init = false;

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (!init)
    {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
        {
            pDevice->GetImmediateContext(&pDeviceContext);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            window = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer = 0;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
            pBackBuffer->Release();
            oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
            ImGui_ImplWin32_Init(window);
            ImGui_ImplDX11_Init(pDevice, pDeviceContext);
            init = true;
        }
        else {
            return oPresent(pSwapChain, SyncInterval, Flags);
        }
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("ImGui Window");
    ImGui::Checkbox("Bhop", &onoroff);
    ImGui::End();

    ImGui::Render();

    pDeviceContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved) {

    // Hook inited bool
    bool init_hook = false;

    do
    {
        if (cha0s::init(cha0s::rendertypes::D3D11) == cha0s::statuses::Success)
        {
            cha0s::bind(8, (void**)&oPresent, hkPresent);
            init_hook = true;
        }
    } while (!init_hook);

    return true;
}

BOOL WINAPI DllMain(const HMODULE instance, const uintptr_t reason, const void* reserved) {

    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(instance);

        const auto MyThread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread), instance, 0, nullptr);
    }

    if (reason == DLL_PROCESS_DETACH) {
        // Shut down the process
    }

    return true;
};