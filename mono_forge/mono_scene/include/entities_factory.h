#pragma once
#include "mono_scene/include/dll_config.h"
#include "riaecs/riaecs.h"

namespace mono_scene
{
    class IEntitiesFactory
    {
    public:
        virtual ~IEntitiesFactory() = default;
        
        virtual void CreateEntities(riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont) = 0;
        virtual void DestroyEntities(riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont) = 0;
    };

} // namespace mono_scene