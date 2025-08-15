#pragma once
#include "mono_d3d12/include/dll_config.h"
#include "riaecs/riaecs.h"

#include "mono_d3d12/include/device_resources.h"

#include <Windows.h>
#include <shared_mutex>

namespace mono_d3d12
{
    constexpr UINT DEFAULT_WINDOW_WIDTH = 960;
    constexpr UINT DEFAULT_WINDOW_HEIGHT = 540;

    constexpr UINT64 INITIAL_FENCE_VALUE = 0;

    constexpr size_t WindowD3D12ComponentMaxCount = 10;
    class MONO_D3D12_API WindowD3D12Component
    {
    public:
        WindowD3D12Component();
        ~WindowD3D12Component();

        std::shared_mutex mutex_;

        HWND handle_ = nullptr;
        HINSTANCE instance_ = nullptr;
        HWND parentHandle_ = nullptr;

        DWORD style_ = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
        std::wstring name_ = L"Window";
        std::wstring className_ = L"WindowClass";
        UINT posX_ = CW_USEDEFAULT;
        UINT posY_ = CW_USEDEFAULT;
        UINT width_ = DEFAULT_WINDOW_WIDTH;
        UINT height_ = DEFAULT_WINDOW_HEIGHT;
        UINT clientWidth_ = 0;
        UINT clientHeight_ = 0;

        bool isCreated_ = false;
        bool isFocused_ = false;
        bool isUnFocused_ = false;
        bool isMaximized_ = false;
        bool isMinimized_ = false;
        bool isFullScreened_ = false;

        bool needsCreate_ = false;
        bool needsShow_ = false;
        bool needsResize_ = false;
        bool needsDestroy_ = false;

        SwapChainContext swapChainContext_;
        RenderTargetContext renderTargetContext_;
        FenceContext fenceContext_;
    };
    extern MONO_D3D12_API riaecs::ComponentRegistrar<WindowD3D12Component, WindowD3D12ComponentMaxCount> WindowD3D12ComponentID;

    MONO_D3D12_API void CreateD3D12Window(WindowD3D12Component *component, WNDCLASSEX &wc);
    MONO_D3D12_API void DestroyD3D12Window(WindowD3D12Component *component);
    // MONO_D3D12_API void Resize(WindowD3D12Component *component, UINT width, UINT height);

    MONO_D3D12_API void UpdateD3D12WindowClientSize(WindowD3D12Component *component);

    MONO_D3D12_API void ShowD3D12Window(WindowD3D12Component *component);
    MONO_D3D12_API void HideD3D12Window(WindowD3D12Component *component);
    // MONO_D3D12_API void Focus(WindowD3D12Component *component);
    // MONO_D3D12_API void UnFocus(WindowD3D12Component *component);
    // MONO_D3D12_API void Maximize(WindowD3D12Component *component);
    // MONO_D3D12_API void Minimize(WindowD3D12Component *component);
    // MONO_D3D12_API void FullScreen(WindowD3D12Component *component);
    // MONO_D3D12_API void Restore(WindowD3D12Component *component);
    MONO_D3D12_API void SetD3D12WindowPosition(WindowD3D12Component *component, UINT x, UINT y);

} // namespace mono_d3d12