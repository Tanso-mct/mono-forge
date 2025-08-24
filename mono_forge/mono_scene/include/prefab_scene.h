#pragma once
#include "mono_scene/include/dll_config.h"
#include "riaecs/riaecs.h"

#include "mono_scene/include/entities_factory.h"

namespace mono_scene
{
    class MONO_SCENE_API PrefabScene : public riaecs::IPrefab
    {
    public:
        PrefabScene() = default;
        ~PrefabScene() override = default;

        PrefabScene(const PrefabScene&) = delete;
        PrefabScene& operator=(const PrefabScene&) = delete;

        std::unique_ptr<IEntitiesFactory> entitiesFactory_ = nullptr;
        std::vector<size_t> assetSourceIDs_;
        std::vector<std::unique_ptr<riaecs::ISystemLoopCommand>> systemListEditCmds_;
        size_t targetEditCmdIndex_ = 0;

        riaecs::Entity Instantiate(riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont) override;
    };

} // namespace mono_scene