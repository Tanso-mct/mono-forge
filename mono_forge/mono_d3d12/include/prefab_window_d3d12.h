#pragma once
#include "mono_d3d12/include/dll_config.h"
#include "riaecs/riaecs.h"

namespace mono_d3d12
{
    class MONO_D3D12_API PrefabWindowD3D12 : public riaecs::IPrefab
    {
    private:
        std::wstring windowName_ = L"Default Window";
        std::wstring windowClassName_ = L"DefaultWindowClass";
        size_t windowWidth_ = 800;
        size_t windowHeight_ = 600;
        size_t windowPosX_ = 100;
        size_t windowPosY_ = 100;
        bool needsShow_ = true;
        bool needsQuitAppWhenDestroyed_ = true;

    public:
        PrefabWindowD3D12() = default;
        ~PrefabWindowD3D12() override = default;

        void SetWindowName(const std::wstring &name) { windowName_ = name; }
        void SetWindowClassName(const std::wstring &className) { windowClassName_ = className; }
        void SetWindowSize(size_t width, size_t height) { windowWidth_ = width; windowHeight_ = height; }
        void SetWindowPosition(size_t posX, size_t posY) { windowPosX_ = posX; windowPosY_ = posY; }
        void SetNeedsShow(bool needsShow) { needsShow_ = needsShow; }
        void SetNeedsQuitAppWhenDestroyed(bool needsQuit) { needsQuitAppWhenDestroyed_ = needsQuit; }

        riaecs::Entity Instantiate(riaecs::IECSWorld &world, riaecs::IAssetContainer &assetCont) override;
    };

} // namespace mono_d3d12