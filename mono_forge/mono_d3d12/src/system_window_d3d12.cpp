#include "mono_d3d12/src/pch.h"
#include "mono_d3d12/include/system_window_d3d12.h"

#include "mono_d3d12/include/component_window_d3d12.h"
#include "mono_d3d12/include/window_message_state.h"

namespace mono_d3d12
{

    LRESULT MONO_D3D12_API CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        mono_d3d12::WindowMessageState& messageState = mono_d3d12::WindowMessageState::GetInstance();
        messageState.AddMessage(hWnd, msg, wParam, lParam);

        switch (msg)
        {
        case WM_PAINT:
            break;

        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }

        return 0;
    }

} // namespace mono_d3d12

mono_d3d12::SystemWindowD3D12::SystemWindowD3D12()
{
}

mono_d3d12::SystemWindowD3D12::~SystemWindowD3D12()
{
}

bool mono_d3d12::SystemWindowD3D12::Update
(
    riaecs::IECSWorld &world, riaecs::IAssetContainer &assetCont, 
    riaecs::ISystemLoopCommandQueue &systemLoopCmdQueue
){
    for (riaecs::Entity entity : world.View(mono_d3d12::ComponentWindowD3D12ID())())
    {
        riaecs::ReadOnlyObject<mono_d3d12::WindowD3D12Component*> window
        = riaecs::GetComponent<mono_d3d12::WindowD3D12Component>(world, entity, mono_d3d12::ComponentWindowD3D12ID());

        std::unique_lock<std::shared_mutex> windowLock(window()->mutex_);

        /***************************************************************************************************************
         * Checks to see if a specific process is needed and executes the required process.
        /**************************************************************************************************************/

        if (window()->needsDestroy_)
            mono_d3d12::DestroyD3D12Window(window());

        if (window()->needsResize_)
            mono_d3d12::ResizeD3D12Window(window(), window()->width_, window()->height_);

        if (window()->needsShow_)
            mono_d3d12::ShowD3D12Window(window());

        if (window()->needsHide_)
            mono_d3d12::HideD3D12Window(window());

        if (window()->needsFocus_)
            mono_d3d12::FocusD3D12Window(window());

        if (window()->needsUnFocus_)
            mono_d3d12::UnFocusD3D12Window(window());

        if (window()->needsMaximize_)
            mono_d3d12::MaximizeD3D12Window(window());

        if (window()->needsMinimize_)
            mono_d3d12::MinimizeD3D12Window(window());

        if (window()->needsFullScreen_)
            mono_d3d12::FullScreenD3D12Window(window());

        if (window()->needsRestore_)
            mono_d3d12::RestoreD3D12Window(window());

        if (window()->needsMove_)
            mono_d3d12::SetD3D12WindowPosition(window(), window()->newPosX_, window()->newPosY_);

        /***************************************************************************************************************
         * Process messages received by WindowProc
        /**************************************************************************************************************/

        mono_d3d12::WindowMessageState& messageState = mono_d3d12::WindowMessageState::GetInstance();
        std::vector<mono_d3d12::WindowMessage> windowMessages = messageState.TakeMessages(window()->handle_);

        for (const mono_d3d12::WindowMessage& windowMessage : windowMessages)
        {
            switch (windowMessage.message)
            {
            case WM_DESTROY:
                mono_d3d12::DestroyedD3D12Window(window());
                if (window()->needsQuitAppWhenDestroyed_)
                    return false; // Stop the system loop

                break;

            case WM_SETFOCUS:
                mono_d3d12::FocusedD3D12Window(window());
                break;

            case WM_KILLFOCUS:
                mono_d3d12::UnFocusedD3D12Window(window());
                break;

            case WM_SIZE:
                if (windowMessage.wParam == SIZE_MAXIMIZED) mono_d3d12::MaximizedD3D12Window(window());
                else if (windowMessage.wParam == SIZE_MINIMIZED) mono_d3d12::MinimizedD3D12Window(window());
                else if (windowMessage.wParam == SIZE_RESTORED) mono_d3d12::RestoredD3D12Window(window());
                mono_d3d12::ResizedD3D12Window(window());

            case WM_MOVE:
                mono_d3d12::MovedD3D12Window(window());
                break;

            default:
                break;
            };
        }
    }

    return true; // Continue the system loop
}

MONO_D3D12_API riaecs::SystemFactoryRegistrar<mono_d3d12::SystemWindowD3D12> mono_d3d12::SystemWindowD3D12ID;