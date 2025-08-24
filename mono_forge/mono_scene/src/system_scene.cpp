#include "mono_scene/src/pch.h"
#include "mono_scene/include/system_scene.h"

#include "mono_scene/include/component_scene.h"

#pragma comment(lib, "riaecs.lib")

mono_scene::SystemScene::SystemScene()
{
}

mono_scene::SystemScene::~SystemScene()
{
}

bool mono_scene::SystemScene::Update
(
    riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont, 
    riaecs::ISystemLoopCommandQueue &systemLoopCmdQueue
){
    for (riaecs::Entity entity : ecsWorld.View(mono_scene::ComponentSceneID())())
    {
        riaecs::ReadOnlyObject<mono_scene::ComponentScene*> scene
        = riaecs::GetComponent<mono_scene::ComponentScene>(ecsWorld, entity, mono_scene::ComponentSceneID());

        std::unique_lock<std::shared_mutex> sceneLock(scene()->mutex_);

        if (scene()->needsLoad_ && riaecs::CheckFutureIsReady(loadFuture))
        {
            loadFuture = std::async
            (
                std::launch::async, &mono_scene::LoadScene, 
                scene(), std::ref(ecsWorld), std::ref(assetCont)
            );
        } 

        if (scene()->needsRelease_ && riaecs::CheckFutureIsReady(releaseFuture))
        {
            releaseFuture = std::async
            (
                std::launch::async, &mono_scene::ReleaseScene, 
                scene(), std::ref(ecsWorld), std::ref(assetCont)
            );
        }

        if (scene()->needsEditSystemList_ && scene()->isLoaded_)
            AddSystemListEditCommand(scene(), systemLoopCmdQueue);
    }

    return true;
}

MONO_SCENE_API riaecs::SystemFactoryRegistrar<mono_scene::SystemScene> mono_scene::SystemSceneID;