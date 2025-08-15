#include "mono_d3d12_test/pch.h"

#include "mono_d3d12/include/device_resources.h"
#pragma comment(lib, "mono_d3d12.lib")

TEST(DeviceResources, GetInstance)
{
    riaecs::ReadWriteObject<mono_d3d12::D3D12DeviceResource> deviceResources 
    = mono_d3d12::D3D12DeviceResource::GetInstance();

    EXPECT_NE(deviceResources().factory_.Get(), nullptr);
    EXPECT_NE(deviceResources().device_.Get(), nullptr);
    EXPECT_NE(deviceResources().commandQueue_.Get(), nullptr);
}