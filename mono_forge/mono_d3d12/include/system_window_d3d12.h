﻿#pragma once
#include "mono_d3d12/include/dll_config.h"
#include "riaecs/riaecs.h"

#include "mono_input_monitor/input_monitor.h"

namespace mono_d3d12
{
    LRESULT MONO_D3D12_API CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    class MONO_D3D12_API SystemWindowD3D12 : public riaecs::ISystem
    {
    private:
        mono_input_monitor::KeyCodeConverter keyCodeConverter_;
        mono_input_monitor::KeyInputConverter keyInputConverter_;
        
        mono_input_monitor::MouseCodeConverter mouseCodeConverter_;
        mono_input_monitor::MouseInputConverter mouseInputConverter_;

    public:
        SystemWindowD3D12();
        ~SystemWindowD3D12() override;

        /***************************************************************************************************************
         * ISystem Implementation
        /**************************************************************************************************************/

        bool Update
        (
            riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont, 
            riaecs::ISystemLoopCommandQueue &systemLoopCmdQueue
        ) override;
    };
    extern MONO_D3D12_API riaecs::SystemFactoryRegistrar<SystemWindowD3D12> SystemWindowD3D12ID;

} // namespace mono_d3d12