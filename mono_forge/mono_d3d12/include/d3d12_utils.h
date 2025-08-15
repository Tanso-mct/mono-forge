#pragma once
#include "mono_d3d12/include/dll_config.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <string>
#include <vector>

namespace mono_d3d12
{
    MONO_D3D12_API void ConvertToHRtoStr(HRESULT hr, std::string &hrStr);

    /*******************************************************************************************************************
     * Related to DirectX 12 Factory and Device
    /******************************************************************************************************************/

    MONO_D3D12_API void CreateDX12Factory(Microsoft::WRL::ComPtr<IDXGIFactory4> &factory);

    MONO_D3D12_API void CreateDX12Device
    (
        Microsoft::WRL::ComPtr<ID3D12Device4> &device, D3D_FEATURE_LEVEL& dstFeatureLevel,
        Microsoft::WRL::ComPtr<IDXGIFactory4> &factory
    );

    MONO_D3D12_API void CreateDX12CommandQueue
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, 
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> &commandQueue
    );

    /*******************************************************************************************************************
     * Related to naming DirectX 12 objects
    /******************************************************************************************************************/

    MONO_D3D12_API void SetName(const Microsoft::WRL::ComPtr<ID3D12Resource> &object, const std::wstring& name);

    /*******************************************************************************************************************
     * Related to SwapChain
    /******************************************************************************************************************/

    MONO_D3D12_API void CreateSwapChain
    (
        const Microsoft::WRL::ComPtr<IDXGIFactory4> &factory, 
        const Microsoft::WRL::ComPtr<ID3D12CommandQueue> &commandQueue,
        const UINT &frameCount, UINT &frameIndex,
        HWND hWnd, const UINT &clientWidth, const UINT &clientHeight,
        Microsoft::WRL::ComPtr<IDXGISwapChain3> &swapChain
    );

    MONO_D3D12_API void ResizeSwapChain
    (
        const UINT &frameCount, UINT &frameIndex,
        const UINT &clientWidth, const UINT &clientHeight,
        const Microsoft::WRL::ComPtr<IDXGISwapChain3> &swapChain
    );

    MONO_D3D12_API void GetBuffersFromSwapChain
    (
        const Microsoft::WRL::ComPtr<IDXGISwapChain3> &swapChain, const UINT& frameCount,
        std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> &buffers
    );

    /*******************************************************************************************************************
     * Related to RenderTargetView
    /******************************************************************************************************************/

    MONO_D3D12_API void CreateRenderTargetViewHeap
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, 
        const UINT& descriptorCount, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, UINT& rtvDescriptorSize
    );

    MONO_D3D12_API void CreateRenderTargetView
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, 
        const UINT& descriptorCount, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> &renderTargets,
        const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &rtvHeap, UINT rtvDescriptorSize
    );

    /*******************************************************************************************************************
     * Related to CommandAllocator
    /******************************************************************************************************************/

    MONO_D3D12_API void CreateCommandAllocator
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, 
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> &commandAllocator
    );

    /*******************************************************************************************************************
     * Related to DepthStencil
    /******************************************************************************************************************/

    MONO_D3D12_API void CreateDepthStencil
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, 
        const UINT& clientWidth, const UINT& clientHeight, Microsoft::WRL::ComPtr<ID3D12Resource>& depthStencil
    );

    MONO_D3D12_API void CreateDepthStencilViewHeap
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, 
        const UINT& depthStencilCount, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, UINT& dsvDescriptorSize
    );

    MONO_D3D12_API void CreateDepthStencilView
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device,
        const UINT &descriptorCount, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> &depthStencils,
        const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &dsvHeap, UINT dsvDescriptorSize
    );

    /*******************************************************************************************************************
     * Related to Viewport and ScissorRect
    /******************************************************************************************************************/

    MONO_D3D12_API void CreateViewport
    (
        D3D12_VIEWPORT& viewport, const UINT& clientWidth, const UINT& clientHeight
    );

    MONO_D3D12_API void CreateScissorRect
    (
        D3D12_RECT& scissorRect, const UINT& clientWidth, const UINT& clientHeight
    );

    /*******************************************************************************************************************
     * Related to Buffer
    /******************************************************************************************************************/

    MONO_D3D12_API void CreateBuffer
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, 
        Microsoft::WRL::ComPtr<ID3D12Resource>& buffer, const UINT& size
    );

    MONO_D3D12_API void UpdateBuffer
    (
        const Microsoft::WRL::ComPtr<ID3D12Resource>& buffer,
        const void* initData, const UINT& size
    );

    /*******************************************************************************************************************
     * Related to Texture2D
    /******************************************************************************************************************/

    MONO_D3D12_API void CreateTexture2D
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device,
        Microsoft::WRL::ComPtr<ID3D12Resource>& texture, 
        const UINT& width, const UINT& height, const DXGI_FORMAT& format, const  D3D12_RESOURCE_FLAGS &flags
    );

    MONO_D3D12_API void CreateUploadHeap
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device,
        Microsoft::WRL::ComPtr<ID3D12Resource>& uploadHeap, const UINT& size
    );

    MONO_D3D12_API UINT GetDXGIFormatByteSize(const DXGI_FORMAT& format);

    MONO_D3D12_API void AddUploadTextureToCmdList
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
        const Microsoft::WRL::ComPtr<ID3D12Resource>& texture, const Microsoft::WRL::ComPtr<ID3D12Resource>& uploadHeap, 
        const void* data, const UINT& width, const UINT& height, const DXGI_FORMAT& format
    );

    MONO_D3D12_API void CreateTexture2DSRV
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device,
        const Microsoft::WRL::ComPtr<ID3D12Resource> &resource, const DXGI_FORMAT &format,
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &srvHeap, const UINT &slotIndex
    );

    /*******************************************************************************************************************
     * Related to CommandList
    /******************************************************************************************************************/

    MONO_D3D12_API void CreateCommandList
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device,
        const Microsoft::WRL::ComPtr<ID3D12CommandAllocator> &commandAllocator,
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &commandList
    );

    /*******************************************************************************************************************
     * Related to Fence
    /******************************************************************************************************************/

    MONO_D3D12_API void CreateFence
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, 
        Microsoft::WRL::ComPtr<ID3D12Fence> &fence, const UINT64 &initialValue = 0
    );

    MONO_D3D12_API void CreateFenceEvent(HANDLE& fenceEvent);
    MONO_D3D12_API void CloseFenceEvent(HANDLE& fenceEvent);

    /*******************************************************************************************************************
     * Related to Command
    /******************************************************************************************************************/

    MONO_D3D12_API void CloseCommand
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &commandList
    );

    MONO_D3D12_API void ResetCommand
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
        const Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& commandAllocator,
        ID3D12PipelineState *pipelineState
    );

    MONO_D3D12_API void ExecuteCommand
    (
        const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue,
        const UINT& commandListCount, std::vector<ID3D12CommandList*>& commandLists
    );

    MONO_D3D12_API void WaitForGPU
    (
        const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue,
        const Microsoft::WRL::ComPtr<ID3D12Fence>& fence, UINT64& fenceValue, HANDLE& fenceEvent
    );

    MONO_D3D12_API void WaitFrameForGPU
    (
        const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue,
        const Microsoft::WRL::ComPtr<ID3D12Fence>& fence, HANDLE& fenceEvent,
        const UINT64& signalFenceValue, UINT64 &waitFenceValue
    );

    MONO_D3D12_API void SetBarrier
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList,
        const Microsoft::WRL::ComPtr<ID3D12Resource> &resource,
        const D3D12_RESOURCE_STATES &beforeState, const D3D12_RESOURCE_STATES &afterState
    );

    MONO_D3D12_API void SetRenderTargets
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList,
        const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &rtvHeap,
        const UINT &rtvIndex, const UINT &rtvDescriptorSize,
        const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &dsvHeap,
        const UINT &dsvIndex, const UINT &dsvDescriptorSize
    );

    MONO_D3D12_API void SetRenderTargets
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList,
        const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &rtvHeap,
        const UINT &rtvIndex, const UINT &rtvDescriptorSize
    );

    MONO_D3D12_API void ClearRenderTargetViews
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList,
        const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &rtvHeap,
        const UINT &rtvIndex, const UINT &rtvDescriptorSize,
        const FLOAT (&clearColor)[4]
    );

    MONO_D3D12_API void ClearDepthStencilViews
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList,
        const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &dsvHeap,
        const UINT &dsvIndex, const UINT &dsvDescriptorSize,
        const D3D12_RECT &scissorRect, const float &depth = 1.0f, const UINT8 &stencil = 0
    );

} // namespace mono_d3d12