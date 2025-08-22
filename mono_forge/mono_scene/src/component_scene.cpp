#include "mono_scene/src/pch.h"
#include "mono_scene/include/component_scene.h"

mono_scene::SceneComponent::SceneComponent()
{
}

mono_scene::SceneComponent::~SceneComponent()
{
    entitiesFactory_.reset();
    assetSourceIDs_.clear();
    systemListEditCmdRegistry_.reset();

    isLoaded_ = false;
    isReleased_ = false;
    isSystemListEdited_ = false;

    needsLoad_ = false;
    needsRelease_ = false;

    needsEditSystemList_ = false;
    targetEditCmdID_ = 0;
}

MONO_SCENE_API riaecs::ComponentRegistrar
<mono_scene::SceneComponent, mono_scene::SceneComponentMaxCount> mono_scene::ComponentSceneID;

MONO_SCENE_API void mono_scene::LoadScene(SceneComponent *component, riaecs::IECSWorld &world, riaecs::IAssetContainer &assetCont)
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
        component->entitiesFactory_->CreateEntities(world, assetCont);

    component->isLoaded_ = true;
    component->needsLoad_ = false;
}

MONO_SCENE_API void mono_scene::ReleaseScene(SceneComponent *component, riaecs::IECSWorld &world, riaecs::IAssetContainer &assetCont)
{
    if (component->isReleased_)
        riaecs::NotifyError({"Scene is already released."}, RIAECS_LOG_LOC);

    // Destroy all entities created by the entities factory
    if (component->entitiesFactory_)
        component->entitiesFactory_->DestroyEntities(world, assetCont);

    // Clear all asset sources
    for (size_t assetSourceID : component->assetSourceIDs_)
    {
        riaecs::ID assetID(assetSourceID, assetCont.GetGeneration(assetSourceID));
        assetCont.Release(assetID);
    }

    component->isReleased_ = true;
    component->needsRelease_ = false;
}

MONO_SCENE_API void mono_scene::AddSystemListEditCommand(SceneComponent *component, riaecs::ISystemLoopCommandQueue &systemLoopCmdQueue)
{
    if (!component->systemListEditCmdRegistry_)
        riaecs::NotifyError({"System list edit command registry is not initialized."}, RIAECS_LOG_LOC);

    std::unique_ptr<riaecs::ISystemLoopCommand> cmd 
    = component->systemListEditCmdRegistry_->Get(component->targetEditCmdID_)().Clone();

    if (!cmd)
        riaecs::NotifyError({"Failed to clone system list edit command."}, RIAECS_LOG_LOC);

    systemLoopCmdQueue.Enqueue(std::move(cmd));

    component->isSystemListEdited_ = true;
    component->needsEditSystemList_ = false;
}