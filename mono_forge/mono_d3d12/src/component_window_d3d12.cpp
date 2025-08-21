#include "mono_d3d12/src/pch.h"
#include "mono_d3d12/include/component_window_d3d12.h"

#include "mono_d3d12/include/d3d12_utils.h"
#include "mono_d3d12/include/device_resources.h"
#include "mono_d3d12/include/system_window_d3d12.h"

mono_d3d12::WindowD3D12Component::WindowD3D12Component()
{
}

mono_d3d12::WindowD3D12Component::~WindowD3D12Component()
{
    handle_ = nullptr;
    instance_ = nullptr;
    parentHandle_ = nullptr;

    style_ = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    name_ = L"Window";
    className_ = L"WindowClass";
    posX_ = CW_USEDEFAULT;
    posY_ = CW_USEDEFAULT;
    width_ = DEFAULT_WINDOW_WIDTH;
    height_ = DEFAULT_WINDOW_HEIGHT;
    clientWidth_ = 0;
    clientHeight_ = 0;

    isCreated_ = false;
    isFocused_ = false;
    isUnFocused_ = false;
    isMaximized_ = false;
    isMinimized_ = false;
    isFullScreened_ = false;

    needsShow_ = false;
    needsResize_ = false;
    needsDestroy_ = false;

    swapChainContext_.~SwapChainContext();
    renderTargetContext_.~RenderTargetContext();
    fenceContext_.~FenceContext();
}

MONO_D3D12_API riaecs::ComponentRegistrar
<mono_d3d12::WindowD3D12Component, mono_d3d12::WindowD3D12ComponentMaxCount> mono_d3d12::ComponentWindowD3D12ID;

MONO_D3D12_API void mono_d3d12::CreateD3D12Window(WindowD3D12Component *component)
{
    if (component->isCreated_)
        riaecs::NotifyError({"This window is already created."}, RIAECS_LOG_LOC);

    if (component->swapChainContext_.frameCount_ == 0 || component->renderTargetContext_.renderTargetCount_ == 0)
        riaecs::NotifyError({"Swap chain frame count and render target count must be greater than zero."}, RIAECS_LOG_LOC);

    if (component->swapChainContext_.frameCount_ != component->renderTargetContext_.renderTargetCount_)
        riaecs::NotifyError({"Swap chain frame count must match render target count."}, RIAECS_LOG_LOC);
    
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = mono_d3d12::WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = component->className_.c_str();
    if (!RegisterClassEx(&wc))
        riaecs::NotifyError({"Failed to register window class."}, RIAECS_LOG_LOC);

    component->handle_ = CreateWindow
    (
        component->className_.c_str(), component->name_.c_str(), component->style_,
        component->posX_, component->posY_, component->width_, component->height_,
        component->parentHandle_, nullptr, wc.hInstance, nullptr
    );
    if (!component->handle_)
        riaecs::NotifyError({"Failed to create window."}, RIAECS_LOG_LOC);

    component->instance_ = wc.hInstance;
    component->isCreated_ = true;

    if (!UpdateWindow(component->handle_))
        riaecs::NotifyError({"Failed to update window."}, RIAECS_LOG_LOC);

    mono_d3d12::UpdateD3D12WindowSize(component);
    mono_d3d12::UpdateD3D12WindowClientSize(component);
    mono_d3d12::SetD3D12WindowPosition(component, component->posX_, component->posY_);

    riaecs::ReadWriteObject<mono_d3d12::D3D12DeviceResource> deviceResource
    = mono_d3d12::D3D12DeviceResource::GetInstance();

    mono_d3d12::CreateSwapChain
    (
        deviceResource().factory_, deviceResource().commandQueue_,
        component->swapChainContext_.frameCount_, component->swapChainContext_.frameIndex_,
        component->handle_, component->clientWidth_, component->clientHeight_,
        component->swapChainContext_.swapChain_
    );

    component->renderTargetContext_.renderTargets_.resize(component->swapChainContext_.frameCount_);
    mono_d3d12::GetBuffersFromSwapChain
    (
        component->swapChainContext_.swapChain_, component->swapChainContext_.frameCount_,
        component->renderTargetContext_.renderTargets_
    );

    for (UINT i = 0; i < component->swapChainContext_.frameCount_; ++i)
        mono_d3d12::SetName
        (
            component->renderTargetContext_.renderTargets_[i].Get(),
            component->name_ + L" Render Target " + std::to_wstring(i + 1)
        );

    mono_d3d12::CreateRenderTargetViewHeap
    (
        deviceResource().device_, component->renderTargetContext_.renderTargetCount_,
        component->renderTargetContext_.rtvDescriptorHeap_,
        component->renderTargetContext_.rtvDescriptorSize_
    );

    mono_d3d12::CreateRenderTargetView
    (
        deviceResource().device_, 
        component->renderTargetContext_.renderTargetCount_, component->renderTargetContext_.renderTargets_, 
        component->renderTargetContext_.rtvDescriptorHeap_, component->renderTargetContext_.rtvDescriptorSize_
    );

    component->renderTargetContext_.depthStencils_.resize(component->renderTargetContext_.depthStencilCount_);
    for (UINT i = 0; i < component->renderTargetContext_.depthStencilCount_; ++i)
    {
        mono_d3d12::CreateDepthStencil
        (
            deviceResource().device_, component->clientWidth_, component->clientHeight_,
            component->renderTargetContext_.depthStencils_[i]
        );

        mono_d3d12::SetName
        (
            component->renderTargetContext_.depthStencils_[i].Get(),
            component->name_ + L" Depth Stencil " + std::to_wstring(i + 1)
        );
    }

    mono_d3d12::CreateDepthStencilViewHeap
    (
        deviceResource().device_, component->renderTargetContext_.depthStencilCount_,
        component->renderTargetContext_.dsvDescriptorHeap_, component->renderTargetContext_.dsvDescriptorSize_
    );

    mono_d3d12::CreateDepthStencilView
    (
        deviceResource().device_, 
        component->renderTargetContext_.depthStencilCount_, component->renderTargetContext_.depthStencils_, 
        component->renderTargetContext_.dsvDescriptorHeap_, component->renderTargetContext_.dsvDescriptorSize_
    );

    mono_d3d12::CreateViewport
    (
        component->renderTargetContext_.viewPort_, 
        component->clientWidth_, component->clientHeight_
    );

    mono_d3d12::CreateScissorRect
    (
        component->renderTargetContext_.scissorRect_, 
        component->clientWidth_, component->clientHeight_
    );

    component->renderTargetContext_.commandAllocators_.resize(component->renderTargetContext_.renderTargetCount_);
    component->renderTargetContext_.commandLists_.resize(component->renderTargetContext_.renderTargetCount_);
    for (UINT i = 0; i < component->renderTargetContext_.renderTargetCount_; ++i)
    {
        mono_d3d12::CreateCommandAllocator
        (
            deviceResource().device_, component->renderTargetContext_.commandAllocators_[i]
        );

        mono_d3d12::CreateCommandList
        (
            deviceResource().device_, component->renderTargetContext_.commandAllocators_[i],
            component->renderTargetContext_.commandLists_[i]
        );
    }

    component->fenceContext_.fenceValues_.resize(component->swapChainContext_.frameCount_, mono_d3d12::INITIAL_FENCE_VALUE);
    mono_d3d12::CreateFence(deviceResource().device_, component->fenceContext_.fence_, INITIAL_FENCE_VALUE);
    mono_d3d12::CreateFenceEvent(component->fenceContext_.fenceEvent_);
}

MONO_D3D12_API void mono_d3d12::DestroyD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"This window is not created."}, RIAECS_LOG_LOC);

    if (!DestroyWindow(component->handle_))
        riaecs::NotifyError({"Failed to destroy window."}, RIAECS_LOG_LOC);

    if (!UnregisterClass(component->className_.c_str(), component->instance_))
        riaecs::NotifyError({"Failed to unregister window class."}, RIAECS_LOG_LOC);
}

MONO_D3D12_API void mono_d3d12::DestroyedD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"This window is not created."}, RIAECS_LOG_LOC);

    riaecs::ReadWriteObject<mono_d3d12::D3D12DeviceResource> deviceResource
    = mono_d3d12::D3D12DeviceResource::GetInstance();

    mono_d3d12::WaitForGPU
    (
        deviceResource().commandQueue_, 
        component->fenceContext_.fence_,
        component->fenceContext_.fenceValues_[component->swapChainContext_.frameIndex_],
        component->fenceContext_.fenceEvent_
    );

    component->swapChainContext_.swapChain_.Reset();

    for (auto& commandAllocator : component->renderTargetContext_.commandAllocators_)
        commandAllocator.Reset();

    for (auto& commandList : component->renderTargetContext_.commandLists_)
        commandList.Reset();

    for (auto& renderTarget : component->renderTargetContext_.renderTargets_)
        renderTarget.Reset();
    component->renderTargetContext_.rtvDescriptorHeap_.Reset();

    for (auto& depthStencil : component->renderTargetContext_.depthStencils_)
        depthStencil.Reset();
    component->renderTargetContext_.dsvDescriptorHeap_.Reset();

    component->fenceContext_.fence_.Reset();
    for (auto& fenceValue : component->fenceContext_.fenceValues_)
        fenceValue = mono_d3d12::INITIAL_FENCE_VALUE;

    component->needsDestroy_ = false;
    component->isCreated_ = false;
}

MONO_D3D12_API void mono_d3d12::ResizeD3D12Window(WindowD3D12Component *component, UINT width, UINT height)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before resizing."}, RIAECS_LOG_LOC);

    if (width == 0 || height == 0)
        riaecs::NotifyError({"Width and height must be greater than zero."}, RIAECS_LOG_LOC);

    component->width_ = width;
    component->height_ = height;

    if (!SetWindowPos(component->handle_, nullptr, component->posX_, component->posY_, width, height, SWP_NOZORDER))
        riaecs::NotifyError({"Failed to set window size."}, RIAECS_LOG_LOC);
}

MONO_D3D12_API void mono_d3d12::ResizedD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before resizing."}, RIAECS_LOG_LOC);

    if (IsIconic(component->handle_))
    {
        component->needsResize_ = false;
        return;
    }

    mono_d3d12::UpdateD3D12WindowSize(component);
    mono_d3d12::UpdateD3D12WindowClientSize(component);

    riaecs::ReadWriteObject<mono_d3d12::D3D12DeviceResource> deviceResource
    = mono_d3d12::D3D12DeviceResource::GetInstance();

    for (auto& renderTarget : component->renderTargetContext_.renderTargets_)
        renderTarget.Reset();

    for (auto& depthStencil : component->renderTargetContext_.depthStencils_)
        depthStencil.Reset();

    for (auto& fenceValue : component->fenceContext_.fenceValues_)
        fenceValue = mono_d3d12::INITIAL_FENCE_VALUE;

    mono_d3d12::ResizeSwapChain
    (
        component->swapChainContext_.frameCount_, component->swapChainContext_.frameIndex_,
        component->clientWidth_, component->clientHeight_,
        component->swapChainContext_.swapChain_
    );

    mono_d3d12::GetBuffersFromSwapChain
    (
        component->swapChainContext_.swapChain_, component->swapChainContext_.frameCount_,
        component->renderTargetContext_.renderTargets_
    );

    for (UINT i = 0; i < component->swapChainContext_.frameCount_; ++i)
        mono_d3d12::SetName
        (
            component->renderTargetContext_.renderTargets_[i].Get(),
            component->name_ + L" Render Target " + std::to_wstring(i + 1)
        );

    mono_d3d12::CreateRenderTargetView
    (
        deviceResource().device_, 
        component->renderTargetContext_.renderTargetCount_, component->renderTargetContext_.renderTargets_, 
        component->renderTargetContext_.rtvDescriptorHeap_, component->renderTargetContext_.rtvDescriptorSize_
    );

    for (UINT i = 0; i < component->renderTargetContext_.depthStencilCount_; ++i)
    {
        mono_d3d12::CreateDepthStencil
        (
            deviceResource().device_, component->clientWidth_, component->clientHeight_,
            component->renderTargetContext_.depthStencils_[i]
        );

        mono_d3d12::SetName
        (
            component->renderTargetContext_.depthStencils_[i].Get(),
            component->name_ + L" Depth Stencil " + std::to_wstring(i + 1)
        );
    }

    mono_d3d12::CreateDepthStencilView
    (
        deviceResource().device_, 
        component->renderTargetContext_.depthStencilCount_, component->renderTargetContext_.depthStencils_, 
        component->renderTargetContext_.dsvDescriptorHeap_, component->renderTargetContext_.dsvDescriptorSize_
    );

    mono_d3d12::CreateViewport
    (
        component->renderTargetContext_.viewPort_, 
        component->clientWidth_, component->clientHeight_
    );

    mono_d3d12::CreateScissorRect
    (
        component->renderTargetContext_.scissorRect_, 
        component->clientWidth_, component->clientHeight_
    );

    component->needsResize_ = false;
}

MONO_D3D12_API void mono_d3d12::UpdateD3D12WindowSize(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before updating size."}, RIAECS_LOG_LOC);

    RECT windowRect;
    if (!GetWindowRect(component->handle_, &windowRect))
        riaecs::NotifyError({"Failed to get window rectangle."}, RIAECS_LOG_LOC);

    component->width_ = windowRect.right - windowRect.left;
    component->height_ = windowRect.bottom - windowRect.top;
}

MONO_D3D12_API void mono_d3d12::UpdateD3D12WindowClientSize(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before updating client size."}, RIAECS_LOG_LOC);

    RECT clientRect;
    if (!GetClientRect(component->handle_, &clientRect))
        riaecs::NotifyError({"Failed to get client rectangle."}, RIAECS_LOG_LOC);

    component->clientWidth_ = clientRect.right - clientRect.left;
    component->clientHeight_ = clientRect.bottom - clientRect.top;
}

MONO_D3D12_API void mono_d3d12::UpdateD3D12WindowPosition(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before updating position."}, RIAECS_LOG_LOC);

    RECT windowRect;
    if (!GetWindowRect(component->handle_, &windowRect))
        riaecs::NotifyError({"Failed to get window rectangle."}, RIAECS_LOG_LOC);

    component->posX_ = windowRect.left;
    component->posY_ = windowRect.top;
}

MONO_D3D12_API void mono_d3d12::ShowD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before showing."}, RIAECS_LOG_LOC);

    ShowWindow(component->handle_, SW_SHOW);
    component->needsShow_ = false;
}

MONO_D3D12_API void mono_d3d12::HideD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before hiding."}, RIAECS_LOG_LOC);

    ShowWindow(component->handle_, SW_HIDE);
    component->needsHide_ = false;
}

MONO_D3D12_API void mono_d3d12::FocusD3D12Window(WindowD3D12Component *component)
{
    mono_d3d12::ShowD3D12Window(component);

    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before focusing."}, RIAECS_LOG_LOC);

    if (!SetForegroundWindow(component->handle_))
        riaecs::NotifyError({"Failed to set window to foreground."}, RIAECS_LOG_LOC);

    if (!SetFocus(component->handle_))
        riaecs::NotifyError({"Failed to set focus to window."}, RIAECS_LOG_LOC);

    component->needsFocus_ = false;
}

MONO_D3D12_API void mono_d3d12::FocusedD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before focusing."}, RIAECS_LOG_LOC);

    component->isFocused_ = true;
}

MONO_D3D12_API void mono_d3d12::UnFocusD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before unfocusing."}, RIAECS_LOG_LOC);

    if (!SetFocus(nullptr))
        riaecs::NotifyError({"Failed to remove focus from window."}, RIAECS_LOG_LOC);

    component->needsUnFocus_ = false;
}

MONO_D3D12_API void mono_d3d12::UnFocusedD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before unfocusing."}, RIAECS_LOG_LOC);

    component->isUnFocused_ = true;
}

MONO_D3D12_API void mono_d3d12::MaximizeD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before maximizing."}, RIAECS_LOG_LOC);

    if (!ShowWindow(component->handle_, SW_MAXIMIZE))
        riaecs::NotifyError({"Failed to maximize window."}, RIAECS_LOG_LOC);

    component->needsMaximize_ = false;
}

MONO_D3D12_API void mono_d3d12::MaximizedD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before maximizing."}, RIAECS_LOG_LOC);

    component->isMaximized_ = true;
}

MONO_D3D12_API void mono_d3d12::MinimizeD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before minimizing."}, RIAECS_LOG_LOC);

    if (!ShowWindow(component->handle_, SW_MINIMIZE))
        riaecs::NotifyError({"Failed to minimize window."}, RIAECS_LOG_LOC);

    component->needsMinimize_ = false;
}

MONO_D3D12_API void mono_d3d12::MinimizedD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before minimizing."}, RIAECS_LOG_LOC);

    component->isMinimized_ = true;
}

MONO_D3D12_API void mono_d3d12::FullScreenD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before full screening."}, RIAECS_LOG_LOC);

    if (!SetWindowLong(component->handle_, GWL_STYLE, WS_POPUP | WS_VISIBLE))
        riaecs::NotifyError({"Failed to set window style for full screen."}, RIAECS_LOG_LOC);

    MONITORINFO mi = {0};
    HMONITOR hMonitor = MonitorFromWindow(component->handle_, MONITOR_DEFAULTTONEAREST);
    mi.cbSize = sizeof(mi);
    if (!GetMonitorInfo(hMonitor, &mi))
        riaecs::NotifyError({"Failed to get monitor info."}, RIAECS_LOG_LOC);

    UINT monitorWidth = mi.rcMonitor.right - mi.rcMonitor.left;
    UINT monitorHeight = mi.rcMonitor.bottom - mi.rcMonitor.top;

    BOOL result = SetWindowPos
    (
        component->handle_, nullptr, 0, 0,
        monitorWidth, monitorHeight, SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOMOVE
    );
    if (!result)
        riaecs::NotifyError({"Failed to set window to full screen."}, RIAECS_LOG_LOC);

    component->needsFullScreen_ = false;
}

MONO_D3D12_API void mono_d3d12::FullScreenedD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before full screening."}, RIAECS_LOG_LOC);

    component->isFullScreened_ = true;
}

MONO_D3D12_API void mono_d3d12::RestoreD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before restoring."}, RIAECS_LOG_LOC);

    if (!SetWindowLong(component->handle_, GWL_STYLE, component->style_))
        riaecs::NotifyError({"Failed to restore window style."}, RIAECS_LOG_LOC);

    component->needsRestore_ = false;
}

MONO_D3D12_API void mono_d3d12::RestoredD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before restoring."}, RIAECS_LOG_LOC);
}

MONO_D3D12_API void mono_d3d12::SetD3D12WindowPosition(WindowD3D12Component *component, UINT x, UINT y)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before setting position."}, RIAECS_LOG_LOC);

    component->posX_ = x;
    component->posY_ = y;

    if (!SetWindowPos(component->handle_, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER))
        riaecs::NotifyError({"Failed to set window position."}, RIAECS_LOG_LOC);

    component->needsMove_ = false;
}

MONO_D3D12_API void mono_d3d12::MovedD3D12Window(WindowD3D12Component *component)
{
    if (!component->isCreated_)
        riaecs::NotifyError({"Window must be created before moving."}, RIAECS_LOG_LOC);

    UpdateD3D12WindowPosition(component);
}