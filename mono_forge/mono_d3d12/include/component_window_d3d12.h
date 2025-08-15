#pragma once
#include "mono_d3d12/include/dll_config.h"
#include "riaecs/riaecs.h"

#include "mono_d3d12/include/device_resources.h"

#include <Windows.h>

namespace mono_d3d12
{
    constexpr UINT DEFAULT_WINDOW_WIDTH = 960;
    constexpr UINT DEFAULT_WINDOW_HEIGHT = 540;

    constexpr size_t WindowD3D12ComponentMaxCount = 10;
    class WindowD3D12Component
    {
    public:
        HWND handle_ = nullptr;
        HINSTANCE instance_ = nullptr;
        HWND parentHandle_ = nullptr;

        DWORD style_ = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
        std::wstring name_ = L"Window";
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
    };
    extern MONO_D3D12_API riaecs::ComponentRegistrar<WindowD3D12Component, WindowD3D12ComponentMaxCount> WindowD3D12ComponentID;

} // namespace mono_d3d12