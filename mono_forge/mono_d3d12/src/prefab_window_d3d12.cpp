#include "mono_d3d12/src/pch.h"
#include "mono_d3d12/include/prefab_window_d3d12.h"

#include "mono_d3d12/include/component_window_d3d12.h"

riaecs::Entity mono_d3d12::PrefabWindowD3D12::Instantiate(riaecs::IECSWorld &world, riaecs::IAssetContainer &assetCont)
{
    riaecs::Entity entity = world.CreateEntity();
    world.AddComponent(entity, ComponentWindowD3D12ID());

    riaecs::ReadOnlyObject<WindowD3D12Component*> window 
    = riaecs::GetComponent<WindowD3D12Component>(world, entity, ComponentWindowD3D12ID());

    std::unique_lock<std::shared_mutex> lock(window()->mutex_);

    window()->name_ = windowName_;
    window()->className_ = windowClassName_;
    window()->width_ = windowWidth_;
    window()->height_ = windowHeight_;
    window()->posX_ = windowPosX_;
    window()->posY_ = windowPosY_;
    window()->needsShow_ = needsShow_;
    window()->needsQuitAppWhenDestroyed_ = needsQuitAppWhenDestroyed_;

    const UINT FRAME_COUNT = 2;
    const UINT DEPTH_STENCIL_COUNT = 1;
    window()->swapChainContext_.frameCount_ = FRAME_COUNT;
    window()->renderTargetContext_.renderTargetCount_ = FRAME_COUNT;
    window()->renderTargetContext_.depthStencilCount_ = DEPTH_STENCIL_COUNT;

    mono_d3d12::CreateD3D12Window(window());

    return entity;
}