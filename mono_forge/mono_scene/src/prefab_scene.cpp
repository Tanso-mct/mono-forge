#include "mono_scene/src/pch.h"
#include "mono_scene/include/prefab_scene.h"

#include "mono_scene/include/component_scene.h"

riaecs::Entity mono_scene::PrefabScene::Instantiate(riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont)
{
    riaecs::Entity entity = ecsWorld.CreateEntity();
    ecsWorld.AddComponent(entity, ComponentSceneID());

    riaecs::ReadOnlyObject<ComponentScene*> scene 
    = riaecs::GetComponent<ComponentScene>(ecsWorld, entity, ComponentSceneID());

    std::unique_lock<std::shared_mutex> lock(scene()->mutex_);

    scene()->entitiesFactory_ = std::move(entitiesFactory_);
    scene()->assetSourceIDs_ = std::move(assetSourceIDs_);
    scene()->systemListEditCmds_ = std::move(systemListEditCmds_);
    scene()->targetEditCmdIndex_ = targetEditCmdIndex_;

    scene()->needsLoad_ = true;
    scene()->needsEditSystemList_ = true;

    return entity;
}