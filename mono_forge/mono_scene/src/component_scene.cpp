#include "mono_scene/src/pch.h"
#include "mono_scene/include/component_scene.h"

mono_scene::ComponentScene::ComponentScene()
{
}

mono_scene::ComponentScene::~ComponentScene()
{
    entitiesFactory_.reset();
    assetSourceIDs_.clear();
    systemListEditCmds_.clear();

    isLoaded_ = false;
    isReleased_ = false;
    isSystemListEdited_ = false;

    needsLoad_ = false;
    needsRelease_ = false;

    needsEditSystemList_ = false;
    targetEditCmdIndex_ = 0;
}

MONO_SCENE_API riaecs::ComponentRegistrar
<mono_scene::ComponentScene, mono_scene::ComponentSceneMaxCount> mono_scene::ComponentSceneID;

MONO_SCENE_API void mono_scene::LoadScene(ComponentScene *component, riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont)
{
    if (component->isLoaded_)
        riaecs::NotifyError({"Scene is already loaded."}, RIAECS_LOG_LOC);

    // Load all file data from asset sources
    std::unordered_map<std::string_view, std::unique_ptr<riaecs::IFileData>> fileDatas;
    for (size_t assetSourceID : component->assetSourceIDs_)
    {
        riaecs::ReadOnlyObject<riaecs::AssetSource> assetSource = riaecs::gAssetSourceRegistry->Get(assetSourceID);

        std::string_view filePath = assetSource().GetFilePath();
        if (fileDatas.find(filePath) == fileDatas.end())
        {
            riaecs::ReadOnlyObject<riaecs::IFileLoader> fileLoader = riaecs::gFileLoaderRegistry->Get(assetSource().GetFileLoaderID());
            fileDatas[filePath] = fileLoader().Load(filePath);
        }
    }

    // Get all asset factory IDs from asset sources
    std::unordered_set<size_t> assetFactoryIDs;
    for (size_t assetSourceID : component->assetSourceIDs_)
    {
        riaecs::ReadOnlyObject<riaecs::AssetSource> assetSource = riaecs::gAssetSourceRegistry->Get(assetSourceID);
        if (assetFactoryIDs.find(assetSource().GetAssetFactoryID()) == assetFactoryIDs.end())
            assetFactoryIDs.insert(assetSource().GetAssetFactoryID());
    }

    // Create all stating areas for assets
    std::unordered_map<size_t, std::unique_ptr<riaecs::IAssetStagingArea>> stagingAreas;
    for (size_t assetFactoryID : assetFactoryIDs)
    {
        riaecs::ReadOnlyObject<riaecs::IAssetFactory> assetFactory = riaecs::gAssetFactoryRegistry->Get(assetFactoryID);
        stagingAreas[assetFactoryID] = assetFactory().Prepare();
    }

    // Create all assets
    for (size_t assetSourceID : component->assetSourceIDs_)
    {
        riaecs::ReadOnlyObject<riaecs::AssetSource> assetSource = riaecs::gAssetSourceRegistry->Get(assetSourceID);
        std::string_view filePath = assetSource().GetFilePath();
        
        if (fileDatas.find(filePath) == fileDatas.end())
            riaecs::NotifyError({"File data not found for path: " + std::string(filePath)}, RIAECS_LOG_LOC);

        riaecs::ReadOnlyObject<riaecs::IAssetFactory> assetFactory = riaecs::gAssetFactoryRegistry->Get(assetSource().GetAssetFactoryID());
        size_t generation = assetCont.GetGeneration(assetSourceID);
        riaecs::ID assetID(assetSourceID, generation);
        assetCont.Set
        (
            assetID, 
            assetFactory().Create(*fileDatas[filePath], *stagingAreas[assetSource().GetAssetFactoryID()])
        );
    }

    // Commit all staging areas
    for (const auto &pair : stagingAreas)
    {
        riaecs::ReadOnlyObject<riaecs::IAssetFactory> assetFactory = riaecs::gAssetFactoryRegistry->Get(pair.first);
        assetFactory().Commit(*pair.second);
    }

    // Create entities using the entities factory
    if (component->entitiesFactory_)
        component->entitiesFactory_->CreateEntities(ecsWorld, assetCont);

    component->isLoaded_ = true;
    component->needsLoad_ = false;
}

MONO_SCENE_API void mono_scene::ReleaseScene(ComponentScene *component, riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont)
{
    if (component->isReleased_)
        riaecs::NotifyError({"Scene is already released."}, RIAECS_LOG_LOC);

    // Destroy all entities created by the entities factory
    if (component->entitiesFactory_)
        component->entitiesFactory_->DestroyEntities(ecsWorld, assetCont);

    // Clear all asset sources
    for (size_t assetSourceID : component->assetSourceIDs_)
    {
        riaecs::ID assetID(assetSourceID, assetCont.GetGeneration(assetSourceID));
        assetCont.Release(assetID);
    }

    component->isReleased_ = true;
    component->needsRelease_ = false;
}

MONO_SCENE_API void mono_scene::AddSystemListEditCommand(ComponentScene *component, riaecs::ISystemLoopCommandQueue &systemLoopCmdQueue)
{
    if (component->targetEditCmdIndex_ >= component->systemListEditCmds_.size())
        riaecs::NotifyError({"Invalid target edit command index."}, RIAECS_LOG_LOC);

    systemLoopCmdQueue.Enqueue(component->systemListEditCmds_[component->targetEditCmdIndex_]->Clone());

    component->isSystemListEdited_ = true;
    component->needsEditSystemList_ = false;
}