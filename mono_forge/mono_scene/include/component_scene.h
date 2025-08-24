#pragma once
#include "mono_scene/include/dll_config.h"
#include "riaecs/riaecs.h"

#include "mono_scene/include/entities_factory.h"

#include <memory>
#include <vector>
#include <shared_mutex>

namespace mono_scene
{
    constexpr size_t ComponentSceneMaxCount = 50;
    class MONO_SCENE_API ComponentScene
    {
    public:
        ComponentScene();
        ~ComponentScene();

        ComponentScene(const ComponentScene&) = delete;
        ComponentScene& operator=(const ComponentScene&) = delete;

        std::shared_mutex mutex_;

        std::unique_ptr<IEntitiesFactory> entitiesFactory_ = nullptr;
        std::vector<size_t> assetSourceIDs_;
        std::vector<std::unique_ptr<riaecs::ISystemLoopCommand>> systemListEditCmds_;

        bool isLoaded_ = false;
        bool isReleased_ = false;
        bool isSystemListEdited_ = false;

        bool needsLoad_ = false;
        bool needsRelease_ = false;

        bool needsEditSystemList_ = false;
        size_t targetEditCmdIndex_ = 0;
    };
    extern MONO_SCENE_API riaecs::ComponentRegistrar<ComponentScene, ComponentSceneMaxCount> ComponentSceneID;

    MONO_SCENE_API void LoadScene(ComponentScene *component, riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont);
    MONO_SCENE_API void ReleaseScene(ComponentScene *component, riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont);

    MONO_SCENE_API void AddSystemListEditCommand(ComponentScene *component, riaecs::ISystemLoopCommandQueue &systemLoopCmdQueue);

}