#pragma once
#include "mono_d3d12/include/dll_config.h"
#include "riaecs/riaecs.h"

namespace mono_d3d12
{
    class MONO_D3D12_API PrefabWindowD3D12 : public riaecs::IPrefab
    {
    public:
        PrefabWindowD3D12() = default;
        ~PrefabWindowD3D12() override = default;

        std::wstring windowName_ = L"Default Window";
        std::wstring windowClassName_ = L"DefaultWindowClass";
        size_t windowWidth_ = 800;
        size_t windowHeight_ = 600;
        size_t windowPosX_ = 100;
        size_t windowPosY_ = 100;
        bool needsShow_ = true;
        bool needsQuitAppWhenDestroyed_ = true;

        riaecs::Entity Instantiate(riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont) override;
    };

} // namespace mono_d3d12