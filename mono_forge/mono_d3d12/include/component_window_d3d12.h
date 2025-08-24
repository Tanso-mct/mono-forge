#pragma once
#include "mono_d3d12/include/dll_config.h"
#include "riaecs/riaecs.h"

#include "mono_d3d12/include/device_resources.h"
#include "mono_d3d12/include/window_message_state.h"

#include "mono_input_monitor/input_monitor.h"

#include <Windows.h>
#include <shared_mutex>
#include <vector>

namespace mono_d3d12
{
    constexpr UINT DEFAULT_WINDOW_WIDTH = 960;
    constexpr UINT DEFAULT_WINDOW_HEIGHT = 540;

    constexpr UINT64 INITIAL_FENCE_VALUE = 0;

    constexpr size_t ComponentWindowD3D12MaxCount = 10;
    class MONO_D3D12_API ComponentWindowD3D12
    {
    public:
        ComponentWindowD3D12();
        ~ComponentWindowD3D12();

        std::shared_mutex mutex_;

        HWND handle_ = nullptr;
        HINSTANCE instance_ = nullptr;
        HWND parentHandle_ = nullptr;

        DWORD style_ = WS_OVERLAPPEDWINDOW;
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

        bool needsDestroy_ = false;
        bool needsResize_ = false;
        bool needsShow_ = false;
        bool needsHide_ = false;
        bool needsFocus_ = false;
        bool needsUnFocus_ = false;
        bool needsMaximize_ = false;
        bool needsMinimize_ = false;
        bool needsFullScreen_ = false;
        bool needsRestore_ = false;
        bool needsQuitAppWhenDestroyed_ = false;

        bool needsMove_ = false;
        UINT newPosX_ = 0;
        UINT newPosY_ = 0;

        SwapChainContext swapChainContext_;
        RenderTargetContext renderTargetContext_;
        FenceContext fenceContext_;

        mono_input_monitor::KeyboardInputState keyboardState_;
        mono_input_monitor::MouseInputState mouseState_;
    };
    extern MONO_D3D12_API riaecs::ComponentRegistrar<ComponentWindowD3D12, ComponentWindowD3D12MaxCount> ComponentWindowD3D12ID;

    MONO_D3D12_API void CreateD3D12Window(ComponentWindowD3D12 *component);

    MONO_D3D12_API void DestroyD3D12Window(ComponentWindowD3D12 *component);
    MONO_D3D12_API void DestroyedD3D12Window(ComponentWindowD3D12 *component);

    MONO_D3D12_API void ResizeD3D12Window(ComponentWindowD3D12 *component, UINT width, UINT height);
    MONO_D3D12_API void ResizedD3D12Window(ComponentWindowD3D12 *component);

    MONO_D3D12_API void UpdateD3D12WindowSize(ComponentWindowD3D12 *component);
    MONO_D3D12_API void UpdateD3D12WindowClientSize(ComponentWindowD3D12 *component);
    MONO_D3D12_API void UpdateD3D12WindowPosition(ComponentWindowD3D12 *component);

    MONO_D3D12_API void ShowD3D12Window(ComponentWindowD3D12 *component);
    MONO_D3D12_API void HideD3D12Window(ComponentWindowD3D12 *component);

    MONO_D3D12_API void FocusD3D12Window(ComponentWindowD3D12 *component);
    MONO_D3D12_API void FocusedD3D12Window(ComponentWindowD3D12 *component);

    MONO_D3D12_API void UnFocusD3D12Window(ComponentWindowD3D12 *component);
    MONO_D3D12_API void UnFocusedD3D12Window(ComponentWindowD3D12 *component);

    MONO_D3D12_API void MaximizeD3D12Window(ComponentWindowD3D12 *component);
    MONO_D3D12_API void MaximizedD3D12Window(ComponentWindowD3D12 *component);

    MONO_D3D12_API void MinimizeD3D12Window(ComponentWindowD3D12 *component);
    MONO_D3D12_API void MinimizedD3D12Window(ComponentWindowD3D12 *component);

    MONO_D3D12_API void FullScreenD3D12Window(ComponentWindowD3D12 *component);
    MONO_D3D12_API void FullScreenedD3D12Window(ComponentWindowD3D12 *component);

    MONO_D3D12_API void RestoreD3D12Window(ComponentWindowD3D12 *component);
    MONO_D3D12_API void RestoredD3D12Window(ComponentWindowD3D12 *component);

    MONO_D3D12_API void SetD3D12WindowPosition(ComponentWindowD3D12 *component, UINT x, UINT y);
    MONO_D3D12_API void MovedD3D12Window(ComponentWindowD3D12 *component);

} // namespace mono_d3d12