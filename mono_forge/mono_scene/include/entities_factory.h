#pragma once
#include "mono_scene/include/dll_config.h"
#include "riaecs/riaecs.h"

namespace mono_scene
{
    class IEntitiesFactory
    {
    public:
        virtual ~IEntitiesFactory() = default;
        
        virtual void CreateEntities(riaecs::IECSWorld &world, riaecs::IAssetContainer &assetCont) const = 0;
        virtual void DestroyEntities(riaecs::IECSWorld &world, riaecs::IAssetContainer &assetCont) const = 0;
    };

} // namespace mono_scene