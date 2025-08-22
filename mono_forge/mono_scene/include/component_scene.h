#pragma once
#include "mono_scene/include/dll_config.h"
#include "riaecs/riaecs.h"

#include "mono_scene/include/entities_factory.h"
#include "mono_scene/include/system_lists.h"

#include <memory>
#include <vector>

namespace mono_scene
{
    constexpr size_t SceneComponentMaxCount = 50;
    class MONO_SCENE_API SceneComponent
    {
    public:
        SceneComponent();
        ~SceneComponent();

        SceneComponent(const SceneComponent&) = delete;
        SceneComponent& operator=(const SceneComponent&) = delete;

        std::unique_ptr<IEntitiesFactory> entitiesFactory_ = nullptr;
        std::vector<size_t> assetSourceIDs_;
        std::unique_ptr<riaecs::IRegistry<ISystemListEditCommand>> systemListEditCmdRegistry_ = nullptr;

        bool isLoaded_ = false;
        bool isReleased_ = false;
        bool isSystemListEdited_ = false;

        bool needsLoad_ = false;
        bool needsRelease_ = false;

        bool needsEditSystemList_ = false;
        size_t targetEditCmdID_ = 0;
    };
    extern MONO_SCENE_API riaecs::ComponentRegistrar<SceneComponent, SceneComponentMaxCount> ComponentSceneID;

    MONO_SCENE_API void LoadScene(SceneComponent *component, riaecs::IECSWorld &world, riaecs::IAssetContainer &assetCont);
    MONO_SCENE_API void ReleaseScene(SceneComponent *component, riaecs::IECSWorld &world, riaecs::IAssetContainer &assetCont);

    MONO_SCENE_API void AddSystemListEditCommand(SceneComponent *component, riaecs::ISystemLoopCommandQueue &systemLoopCmdQueue);

}