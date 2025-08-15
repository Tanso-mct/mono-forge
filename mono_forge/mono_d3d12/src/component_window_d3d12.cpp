#include "mono_d3d12/src/pch.h"
#include "mono_d3d12/include/component_window_d3d12.h"

MONO_D3D12_API riaecs::ComponentRegistrar
<mono_d3d12::WindowD3D12Component, mono_d3d12::WindowD3D12ComponentMaxCount> mono_d3d12::WindowD3D12ComponentID;