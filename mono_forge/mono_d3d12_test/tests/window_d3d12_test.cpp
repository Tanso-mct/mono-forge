#include "mono_d3d12_test/pch.h"

#include "mono_d3d12/include/component_window_d3d12.h"
#pragma comment(lib, "mono_d3d12.lib")

static LRESULT CALLBACK WindowProcWindowD3D12(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

TEST(WindowD3D12, Create)
{
    mono_d3d12::WindowD3D12Component windowComponent;
    windowComponent.name_ = L"Test Window";
    windowComponent.className_ = L"TestWindowClass";
    windowComponent.width_ = 800;
    windowComponent.height_ = 600;
    windowComponent.posX_ = 100;
    windowComponent.posY_ = 100;

    const UINT FRAME_COUNT = 2;
    const UINT DEPTH_STENCIL_COUNT = 1;
    windowComponent.swapChainContext_.frameCount_ = FRAME_COUNT;
    windowComponent.renderTargetContext_.renderTargetCount_ = FRAME_COUNT;
    windowComponent.renderTargetContext_.depthStencilCount_ = DEPTH_STENCIL_COUNT;

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProcWindowD3D12;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = windowComponent.className_.c_str();

    EXPECT_NO_THROW(mono_d3d12::CreateD3D12Window(&windowComponent, wc));
    EXPECT_TRUE(windowComponent.isCreated_);
    EXPECT_NE(windowComponent.handle_, nullptr);

    EXPECT_NO_THROW(mono_d3d12::ShowD3D12Window(&windowComponent));

    mono_d3d12::DestroyD3D12Window(&windowComponent);
    EXPECT_FALSE(windowComponent.isCreated_);   
}