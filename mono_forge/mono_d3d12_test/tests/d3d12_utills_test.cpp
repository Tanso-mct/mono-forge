#include "mono_d3d12_test/pch.h"

#include "mono_d3d12/include/d3d12_utils.h"
#pragma comment(lib, "mono_d3d12.lib")

using Microsoft::WRL::ComPtr;

static LRESULT CALLBACK WindowProcD3D12Helpers(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

namespace
{
    void CreateWindowTemp(HWND& hWnd, WNDCLASSEX& wc, std::wstring className)
    {
        wc.lpszClassName = className.c_str();
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpfnWndProc = WindowProcD3D12Helpers;

        if (!RegisterClassEx(&wc))
        {
            FAIL() << "Failed to register window class: ";
        }

        hWnd = CreateWindow
        (
            wc.lpszClassName,
            className.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            800, 600,
            nullptr, nullptr, wc.hInstance, nullptr
        );
        if (!hWnd)
        {
            FAIL() << "Failed to create window";
        }

        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
    }
}

TEST(DxHelpers, CreateDX12Factory)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);
    EXPECT_NE(factory.Get(), nullptr);
}

TEST(DxHelpers, CreateDX12Device)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    EXPECT_NE(device.Get(), nullptr);
    // EXPECT_EQ(featureLevel, D3D_FEATURE_LEVEL_12_2); // Feature level may vary based on the system
}

TEST(DxHelpers, CreateDX12CommandQueue)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandQueue> commandQueue;
    mono_d3d12::CreateDX12CommandQueue(device, commandQueue);

    EXPECT_NE(commandQueue.Get(), nullptr);
}

TEST(DxHelpers, CreateSwapChain)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandQueue> commandQueue;
    mono_d3d12::CreateDX12CommandQueue(device, commandQueue);

    HWND hWnd = nullptr;
    {
        WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
        wc.lpszClassName = L"CreateSwapChain";
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpfnWndProc = WindowProcD3D12Helpers;
        ::CreateWindowTemp(hWnd, wc, L"CreateSwapChain");
    }

    ComPtr<IDXGISwapChain3> swapChain;
    const UINT frameCount = 2;
    UINT frameIndex = 0;

    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        UINT clientWidth = rect.right - rect.left;
        UINT clientHeight = rect.bottom - rect.top;

        mono_d3d12::CreateSwapChain(factory, commandQueue, frameCount, frameIndex, hWnd, clientWidth, clientHeight, swapChain);
    }

    EXPECT_NE(swapChain.Get(), nullptr);

    DestroyWindow(hWnd);
}

TEST(DxHelpers, ResizeSwapChain)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandQueue> commandQueue;
    mono_d3d12::CreateDX12CommandQueue(device, commandQueue);

    HWND hWnd = nullptr;
    {
        WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
        wc.lpszClassName = L"ResizeSwapChain";
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpfnWndProc = WindowProcD3D12Helpers;
        ::CreateWindowTemp(hWnd, wc, L"ResizeSwapChain");
    }

    ComPtr<IDXGISwapChain3> swapChain;
    const UINT frameCount = 2;
    UINT frameIndex = 0;

    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        UINT clientWidth = rect.right - rect.left;
        UINT clientHeight = rect.bottom - rect.top;

        mono_d3d12::CreateSwapChain(factory, commandQueue, frameCount, frameIndex, hWnd, clientWidth, clientHeight, swapChain);
    }

    EXPECT_NE(swapChain.Get(), nullptr);

    // Edit the window size
    {
        UINT newWidth = 500;
        UINT newHeight = 500;
        SetWindowPos(hWnd, nullptr, 0, 0, newWidth, newHeight, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
    }

    // Resize the swap chain
    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        UINT clientWidth = rect.right - rect.left;
        UINT clientHeight = rect.bottom - rect.top;

        mono_d3d12::ResizeSwapChain(frameCount, frameIndex, clientWidth, clientHeight, swapChain);
    }

    DestroyWindow(hWnd);
}

TEST(DxHelpers, GetBuffersFromSwapChain)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandQueue> commandQueue;
    mono_d3d12::CreateDX12CommandQueue(device, commandQueue);

    HWND hWnd = nullptr;
    {
        WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
        wc.lpszClassName = L"GetBuffersFromSwapChain";
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpfnWndProc = WindowProcD3D12Helpers;
        ::CreateWindowTemp(hWnd, wc, L"GetBuffersFromSwapChain");
    }

    ComPtr<IDXGISwapChain3> swapChain;
    const UINT frameCount = 2;
    UINT frameIndex = 0;

    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        UINT clientWidth = rect.right - rect.left;
        UINT clientHeight = rect.bottom - rect.top;

        mono_d3d12::CreateSwapChain(factory, commandQueue, frameCount, frameIndex, hWnd, clientWidth, clientHeight, swapChain);
    }

    EXPECT_NE(swapChain.Get(), nullptr);

    std::vector<ComPtr<ID3D12Resource>> buffers;
    mono_d3d12::GetBuffersFromSwapChain(swapChain, frameCount, buffers);
    EXPECT_EQ(buffers.size(), frameCount);

    for (int i = 0; i < frameCount; ++i)
    {
        EXPECT_NE(buffers[i].Get(), nullptr);
        std::wstring bufferName = L"Buffer_" + std::to_wstring(i);
        mono_d3d12::SetName(buffers[i], bufferName);
    }

    DestroyWindow(hWnd);
}

TEST(DxHelpers, CreateRenderTargetViewHeap)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12DescriptorHeap> rtvHeap;
    const UINT rtvDescriptorCount = 2; // Create 2 RTVs
    UINT rtvDescriptorSize = 0;

    mono_d3d12::CreateRenderTargetViewHeap(device, rtvDescriptorCount, rtvHeap, rtvDescriptorSize);
    EXPECT_NE(rtvHeap.Get(), nullptr);
}

TEST(DxHelpers, CreateRenderTargetView)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    std::vector<ComPtr<ID3D12Resource>> buffers;
    buffers.resize(1);

    const UINT width = 256;
    const UINT height = 256;
    mono_d3d12::CreateTexture2D(device, buffers[0], width, height, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
    EXPECT_NE(buffers[0].Get(), nullptr);

    ComPtr<ID3D12DescriptorHeap> rtvHeap;
    const UINT rtvDescriptorCount = 1; // Create 1 RTV
    UINT rtvDescriptorSize = 0;

    mono_d3d12::CreateRenderTargetViewHeap(device, rtvDescriptorCount, rtvHeap, rtvDescriptorSize);
    EXPECT_NE(rtvHeap.Get(), nullptr);

    mono_d3d12::CreateRenderTargetView(device, rtvDescriptorCount, buffers, rtvHeap, rtvDescriptorSize);
}

TEST(DxHelpers, CreateCommandAllocator)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandAllocator> commandAllocator;
    mono_d3d12::CreateCommandAllocator(device, commandAllocator);
    EXPECT_NE(commandAllocator.Get(), nullptr);
}

TEST(DxHelpers, CreateDepthStencil)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12Resource> depthStencil;
    const UINT clientWidth = 800;
    const UINT clientHeight = 600;

    mono_d3d12::CreateDepthStencil(device, clientWidth, clientHeight, depthStencil);
    EXPECT_NE(depthStencil.Get(), nullptr);
}

TEST(DxHelpers, CreateDepthStencilViewHeap)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12DescriptorHeap> dsvHeap;
    const UINT depthStencilCount = 1; // Create 1 DSV
    UINT dsvDescriptorSize = 0;

    mono_d3d12::CreateDepthStencilViewHeap(device, depthStencilCount, dsvHeap, dsvDescriptorSize);
    EXPECT_NE(dsvHeap.Get(), nullptr);
}

TEST(DxHelpers, CreateDepthStencilView)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    std::vector<ComPtr<ID3D12Resource>> depthStencils;
    depthStencils.resize(1);

    const UINT clientWidth = 800;
    const UINT clientHeight = 600;

    mono_d3d12::CreateDepthStencil(device, clientWidth, clientHeight, depthStencils[0]);
    EXPECT_NE(depthStencils[0].Get(), nullptr);

    ComPtr<ID3D12DescriptorHeap> dsvHeap;
    const UINT depthStencilCount = 1; // Create 1 DSV
    UINT dsvDescriptorSize = 0;
    mono_d3d12::CreateDepthStencilViewHeap(device, depthStencilCount, dsvHeap, dsvDescriptorSize);
    EXPECT_NE(dsvHeap.Get(), nullptr);

    // Create the DSV
    mono_d3d12::CreateDepthStencilView(device, depthStencilCount, depthStencils, dsvHeap, dsvDescriptorSize);
}

TEST(DxHelpers, CreateViewport)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    D3D12_VIEWPORT viewport;
    const UINT clientWidth = 800;
    const UINT clientHeight = 600;

    mono_d3d12::CreateViewport(viewport, clientWidth, clientHeight);
    EXPECT_EQ(viewport.Width, static_cast<float>(clientWidth));
    EXPECT_EQ(viewport.Height, static_cast<float>(clientHeight));
}

TEST(DxHelpers, CreateScissorRect)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    D3D12_RECT scissorRect;
    const UINT clientWidth = 800;
    const UINT clientHeight = 600;

    mono_d3d12::CreateScissorRect(scissorRect, clientWidth, clientHeight);
    EXPECT_EQ(scissorRect.right - scissorRect.left, static_cast<LONG>(clientWidth));
    EXPECT_EQ(scissorRect.bottom - scissorRect.top, static_cast<LONG>(clientHeight));
}

TEST(DxHelpers, CreateBuffer)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12Resource> buffer;
    const UINT size = 1024; // Create a 1KB buffer

    mono_d3d12::CreateBuffer(device, buffer, size);
    EXPECT_NE(buffer.Get(), nullptr);

    // バッファの名前を設定
    std::wstring bufferName = L"TestBuffer";
    mono_d3d12::SetName(buffer, bufferName);
}

TEST(DxHelpers, UpdateBuffer)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12Resource> buffer;
    const UINT size = 1024; // Create a 1KB buffer
    mono_d3d12::CreateBuffer(device, buffer, size);

    // バッファの名前を設定
    std::wstring bufferName = L"UpdatedBuffer";
    mono_d3d12::SetName(buffer, bufferName);

    // バッファに書き込むデータ
    std::vector<uint8_t> initData(size, 0xAB); // Fill with 0xAB

    mono_d3d12::UpdateBuffer(buffer, initData.data(), size);
}

TEST(DxHelpers, CreateTexture2D)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12Resource> texture;
    const UINT width = 256;
    const UINT height = 256;

    mono_d3d12::CreateTexture2D(device, texture, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_FLAG_NONE);
    EXPECT_NE(texture.Get(), nullptr);

    std::wstring textureName = L"TestTexture";
    mono_d3d12::SetName(texture, textureName);
}

TEST(DxHelpers, CreateUploadHeap)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12Resource> uploadHeap;
    const UINT size = 1024; // Create a 1KB upload heap

    mono_d3d12::CreateUploadHeap(device, uploadHeap, size);
    EXPECT_NE(uploadHeap.Get(), nullptr);

    std::wstring uploadHeapName = L"TestUploadHeap";
    mono_d3d12::SetName(uploadHeap, uploadHeapName);
}

TEST(DxHelpers, AddUploadTextureToCmdList)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandAllocator> commandAllocator;
    mono_d3d12::CreateCommandAllocator(device, commandAllocator);
    EXPECT_NE(commandAllocator.Get(), nullptr);

    ComPtr<ID3D12GraphicsCommandList> commandList;
    mono_d3d12::CreateCommandList(device, commandAllocator, commandList);
    EXPECT_NE(commandList.Get(), nullptr);

    ComPtr<ID3D12Resource> texture;
    const UINT width = 256;
    const UINT height = 256;
    mono_d3d12::CreateTexture2D(device, texture, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_FLAG_NONE);
    EXPECT_NE(texture.Get(), nullptr);

    ComPtr<ID3D12Resource> uploadHeap;
    const UINT uploadHeapSize = 256 * 256 * 4; // RGBA format texture data size
    mono_d3d12::CreateUploadHeap(device, uploadHeap, uploadHeapSize);
    EXPECT_NE(uploadHeap.Get(), nullptr);

    mono_d3d12::ResetCommand(commandList, commandAllocator, nullptr);

    std::vector<uint8_t> textureData(width * height * 4, 0xFF); // White texture data
    mono_d3d12::AddUploadTextureToCmdList
    (
        commandList, texture, uploadHeap, textureData.data(), width, height, DXGI_FORMAT_R8G8B8A8_UNORM
    );
}

TEST(DxHelpers, ResetCommand)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandAllocator> commandAllocator;
    mono_d3d12::CreateCommandAllocator(device, commandAllocator);
    EXPECT_NE(commandAllocator.Get(), nullptr);

    ComPtr<ID3D12GraphicsCommandList> commandList;
    mono_d3d12::CreateCommandList(device, commandAllocator, commandList);
    EXPECT_NE(commandList.Get(), nullptr);

    mono_d3d12::ResetCommand(commandList, commandAllocator, nullptr);
}

TEST(DxHelpers, UploadTexture)
{
    ComPtr<IDXGIFactory4> factory;
    mono_d3d12::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    mono_d3d12::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandQueue> commandQueue;
    mono_d3d12::CreateDX12CommandQueue(device, commandQueue);
    EXPECT_NE(commandQueue.Get(), nullptr);

    ComPtr<ID3D12Fence> fence;
    UINT64 fenceValue = 0;
    mono_d3d12::CreateFence(device, fence, fenceValue);

    HANDLE fenceEvent = nullptr;
    mono_d3d12::CreateFenceEvent(fenceEvent);

    ComPtr<ID3D12CommandAllocator> commandAllocator;
    mono_d3d12::CreateCommandAllocator(device, commandAllocator);
    EXPECT_NE(commandAllocator.Get(), nullptr);

    ComPtr<ID3D12GraphicsCommandList> commandList;
    mono_d3d12::CreateCommandList(device, commandAllocator, commandList);
    EXPECT_NE(commandList.Get(), nullptr);

    ComPtr<ID3D12Resource> texture;
    const UINT width = 256;
    const UINT height = 256;
    mono_d3d12::CreateTexture2D(device, texture, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_FLAG_NONE);
    EXPECT_NE(texture.Get(), nullptr);

    ComPtr<ID3D12Resource> uploadHeap;
    const UINT uploadHeapSize = width * height * 4; // RGBA format texture data size
    mono_d3d12::CreateUploadHeap(device, uploadHeap, uploadHeapSize);
    EXPECT_NE(uploadHeap.Get(), nullptr);

    mono_d3d12::ResetCommand(commandList, commandAllocator, nullptr);

    std::unique_ptr<uint8_t[]> textureData(new uint8_t[width * height * 4]);
    mono_d3d12::AddUploadTextureToCmdList
    (
        commandList, texture, uploadHeap, textureData.get(), width, height, DXGI_FORMAT_R8G8B8A8_UNORM
    );

    mono_d3d12::CloseCommand(commandList);

    mono_d3d12::WaitForGPU(commandQueue, fence, fenceValue, fenceEvent);

    std::vector<ID3D12CommandList*> commandLists;
    commandLists.push_back(commandList.Get());
    mono_d3d12::ExecuteCommand(commandQueue, commandLists.size(), commandLists);

    mono_d3d12::WaitForGPU(commandQueue, fence, fenceValue, fenceEvent);

    mono_d3d12::CloseFenceEvent(fenceEvent);
}