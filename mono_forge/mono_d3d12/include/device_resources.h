#pragma once
#include "mono_d3d12/include/dll_config.h"
#include "riaecs/riaecs.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <shared_mutex>

namespace mono_d3d12
{
    class MONO_D3D12_API D3D12DeviceResources
    {
    private:
        std::shared_mutex mutex_;
    public:
        D3D12DeviceResources() = default;
        ~D3D12DeviceResources() = default;

        Microsoft::WRL::ComPtr<IDXGIFactory4> factory_;
        Microsoft::WRL::ComPtr<ID3D12Device4> device_;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
        D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_12_0; 

        riaecs::ReadWriteObject<D3D12DeviceResources> GetInstance();
    };

    struct FenceContext
    {
        Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
        std::vector<UINT64> fenceValues_;
        HANDLE fenceEvent_;
    };

    struct RenderTargetContext
    {
        std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> commandAllocators_;
        std::vector<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>> commandLists_;

        UINT renderTargetCount_;
        std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> renderTargets_;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_;
        UINT rtvDescriptorSize_;

        UINT depthStencilCount_;
        std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> depthStencils_;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_;
        UINT dsvDescriptorSize_;

        D3D12_VIEWPORT viewPort_;
        D3D12_RECT scissorRect_;
    };

    struct SwapChainContext
    {
        Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain_;
        UINT frameCount_;
        UINT frameIndex_;
        UINT syncInterval_;
    };

} // namespace mono_d3d12