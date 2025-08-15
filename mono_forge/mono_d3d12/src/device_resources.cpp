#include "mono_d3d12/src/pch.h"
#include "mono_d3d12/include/device_resources.h"

#include "mono_d3d12/include/d3d12_utils.h"

MONO_D3D12_API mono_d3d12::DeviceResources& mono_d3d12::DeviceResources::GetInstance()
{
    static DeviceResources instance;
    static bool isInitialized = false;

    if (!isInitialized)
    {
        // Initialize the DirectX 12 factory and device
        mono_d3d12::CreateDX12Factory(instance.factory_);
        mono_d3d12::CreateDX12Device(instance.device_, instance.featureLevel, instance.factory_);
        mono_d3d12::CreateDX12CommandQueue(instance.device_, instance.commandQueue_);

        isInitialized = true;
    }

    return instance;
}