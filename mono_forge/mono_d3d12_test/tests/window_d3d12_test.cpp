#include "mono_d3d12_test/pch.h"

#pragma comment(lib, "riaecs.lib")

#include "mono_d3d12/include/component_window_d3d12.h"
#include "mono_d3d12/include/system_window_d3d12.h"
#include "mono_d3d12/include/window_message_state.h"
#pragma comment(lib, "mono_d3d12.lib")

#include <future>

TEST(WindowD3D12, Create)
{
    /*******************************************************************************************************************
     * Set window component properties
    /******************************************************************************************************************/

    mono_d3d12::WindowD3D12Component windowComponent;
    windowComponent.name_ = L"Test Window";
    windowComponent.className_ = L"TestWindowClass";
    windowComponent.width_ = 800;
    windowComponent.height_ = 600;
    windowComponent.posX_ = 100;
    windowComponent.posY_ = 100;
    windowComponent.needsShow_ = true;
    windowComponent.needsQuitAppWhenDestroyed_ = true;

    const UINT FRAME_COUNT = 2;
    const UINT DEPTH_STENCIL_COUNT = 1;
    windowComponent.swapChainContext_.frameCount_ = FRAME_COUNT;
    windowComponent.renderTargetContext_.renderTargetCount_ = FRAME_COUNT;
    windowComponent.renderTargetContext_.depthStencilCount_ = DEPTH_STENCIL_COUNT;

    /*******************************************************************************************************************
     * Create the window on main thread
    /******************************************************************************************************************/

    mono_d3d12::CreateD3D12Window(&windowComponent);

    /*******************************************************************************************************************
     * Run the main loop to process messages
    /******************************************************************************************************************/

    bool isRunning = true;
    std::future<void> mainLoop = std::async
    (
        std::launch::async, [&windowComponent, &isRunning]()
        {
            while (isRunning)
            {
                // For multithreaded access to the window component
                std::unique_lock<std::shared_mutex> lock(windowComponent.mutex_);

                if (windowComponent.needsDestroy_)
                    mono_d3d12::DestroyD3D12Window(&windowComponent);

                if (windowComponent.needsResize_)
                    mono_d3d12::ResizeD3D12Window(&windowComponent, windowComponent.width_, windowComponent.height_);

                if (windowComponent.needsShow_)
                    mono_d3d12::ShowD3D12Window(&windowComponent);

                if (windowComponent.needsHide_)
                    mono_d3d12::HideD3D12Window(&windowComponent);

                if (windowComponent.needsFocus_)
                    mono_d3d12::FocusD3D12Window(&windowComponent);

                if (windowComponent.needsUnFocus_)
                    mono_d3d12::UnFocusD3D12Window(&windowComponent);

                if (windowComponent.needsMaximize_)
                    mono_d3d12::MaximizeD3D12Window(&windowComponent);

                if (windowComponent.needsMinimize_)
                    mono_d3d12::MinimizeD3D12Window(&windowComponent);

                if (windowComponent.needsFullScreen_)
                    mono_d3d12::FullScreenD3D12Window(&windowComponent);

                if (windowComponent.needsRestore_)
                    mono_d3d12::RestoreD3D12Window(&windowComponent);

                if (windowComponent.needsMove_)
                    mono_d3d12::SetD3D12WindowPosition(&windowComponent, windowComponent.newPosX_, windowComponent.newPosY_);

                /***************************************************************************************************************
                 * Process messages received by WindowProc
                /**************************************************************************************************************/

                mono_d3d12::WindowMessageState& messageState = mono_d3d12::WindowMessageState::GetInstance();
                std::vector<mono_d3d12::WindowMessage> windowMessages = messageState.TakeMessages(windowComponent.handle_);

                for (const mono_d3d12::WindowMessage& windowMessage : windowMessages)
                {
                    switch (windowMessage.message)
                    {
                    case WM_DESTROY:
                        mono_d3d12::DestroyedD3D12Window(&windowComponent);
                        if (windowComponent.needsQuitAppWhenDestroyed_)
                            isRunning = false;
                        break;

                    case WM_SETFOCUS:
                        mono_d3d12::FocusedD3D12Window(&windowComponent);
                        break;

                    case WM_KILLFOCUS:
                        mono_d3d12::UnFocusedD3D12Window(&windowComponent);
                        break;

                    case WM_SIZE:
                        if (windowMessage.wParam == SIZE_MAXIMIZED) mono_d3d12::MaximizedD3D12Window(&windowComponent);
                        else if (windowMessage.wParam == SIZE_MINIMIZED) mono_d3d12::MinimizedD3D12Window(&windowComponent);
                        else if (windowMessage.wParam == SIZE_RESTORED) mono_d3d12::RestoredD3D12Window(&windowComponent);
                        mono_d3d12::ResizedD3D12Window(&windowComponent);

                    case WM_MOVE:
                        mono_d3d12::MovedD3D12Window(&windowComponent);
                        break;

                    default:
                        break;
                    };
                }
            }
        }
    );

    /*******************************************************************************************************************
     * Run the message loop to keep the window responsive
    /******************************************************************************************************************/

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (!isRunning)
            PostQuitMessage(0);
    }

    // Wait for the main loop to finish
    mainLoop.get();
}