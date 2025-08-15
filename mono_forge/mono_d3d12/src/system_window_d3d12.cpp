#include "mono_d3d12/src/pch.h"
#include "mono_d3d12/include/system_window_d3d12.h"

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
    return false;
}

MONO_D3D12_API riaecs::SystemFactoryRegistrar<mono_d3d12::SystemWindowD3D12> mono_d3d12::SystemWindowD3D12ID;