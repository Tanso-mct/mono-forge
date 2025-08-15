#include "mono_d3d12_test/pch.h"

#include "mono_d3d12/include/device_resources.h"
#pragma comment(lib, "mono_d3d12.lib")

TEST(DeviceResource, GetInstance)
{
    riaecs::ReadWriteObject<mono_d3d12::D3D12DeviceResource> deviceResource 
    = mono_d3d12::D3D12DeviceResource::GetInstance();

    EXPECT_NE(deviceResource().factory_.Get(), nullptr);
    EXPECT_NE(deviceResource().device_.Get(), nullptr);
    EXPECT_NE(deviceResource().commandQueue_.Get(), nullptr);
}