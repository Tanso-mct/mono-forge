#pragma once
#include "mono_d3d12/include/dll_config.h"
#include "riaecs/riaecs.h"

namespace mono_d3d12
{
    class MONO_D3D12_API SystemWindowD3D12 : public riaecs::ISystem
    {
    public:
        SystemWindowD3D12();
        ~SystemWindowD3D12() override;

        /***************************************************************************************************************
         * ISystem Implementation
        /**************************************************************************************************************/

        bool Update
        (
            riaecs::IECSWorld &world, riaecs::IAssetContainer &assetCont, 
            riaecs::ISystemLoopCommandQueue &systemLoopCmdQueue
        ) override;
    };
    extern MONO_D3D12_API riaecs::SystemFactoryRegistrar<SystemWindowD3D12> SystemWindowD3D12ID;

} // namespace mono_d3d12