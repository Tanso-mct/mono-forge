#include "mono_d3d12/src/pch.h"
#include "mono_d3d12/include/device_resources.h"

#include "mono_d3d12/include/d3d12_utils.h"

MONO_D3D12_API riaecs::ReadWriteObject<mono_d3d12::D3D12DeviceResource> mono_d3d12::D3D12DeviceResource::GetInstance()
{
    static D3D12DeviceResource instance;
    static bool isInitialized = false;

    if (!isInitialized)
    {
        // Initialize the DirectX 12 factory and device
        mono_d3d12::CreateDX12Factory(instance.factory_);
        mono_d3d12::CreateDX12Device(instance.device_, instance.featureLevel, instance.factory_);
        mono_d3d12::CreateDX12CommandQueue(instance.device_, instance.commandQueue_);

        isInitialized = true;
    }

    std::unique_lock<std::shared_mutex> mainLock(instance.mutex_);
    return riaecs::ReadWriteObject<mono_d3d12::D3D12DeviceResource>(std::move(mainLock), instance);
}