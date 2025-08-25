#include "mono_forge/src/pch.h"

#pragma comment(lib, "riaecs.lib")
#pragma comment(lib, "mem_alloc_fixed_block.lib")

#include "mono_d3d12/mono_d3d12.h"
#pragma comment(lib, "mono_d3d12.lib")

#include "mono_scene/mono_scene.h"
#pragma comment(lib, "mono_scene.lib")

#include "mono_forge/include/game_loop.h"

namespace mono_forge
{
    class InitialSystemListFactory : public riaecs::DefaultSystemListFactory
    {
    public:
        virtual std::unique_ptr<riaecs::ISystemList> Create() const override
        {
            std::unique_ptr<riaecs::ISystemList> systemList = std::make_unique<riaecs::SystemList>();

            systemList->CreateSystem(mono_d3d12::SystemWindowD3D12ID());
            systemList->CreateSystem(mono_scene::SystemSceneID());

            systemList->SetOrder
            ({ 
                mono_d3d12::SystemWindowD3D12ID(),
                mono_scene::SystemSceneID()
            });
            
            return systemList;
        }
    };

    class TitleSceneSystemListEditCmd : public riaecs::ISystemLoopCommand
    {
    public:
        void Execute
        (
            riaecs::ISystemList &systemList, 
            riaecs::IECSWorld &ecsWorld, 
            riaecs::IAssetContainer &assetCont
        ) const override
        {
            if (!systemList.HasSystem(mono_d3d12::SystemWindowD3D12ID()))
                systemList.CreateSystem(mono_d3d12::SystemWindowD3D12ID());

            if (!systemList.HasSystem(mono_scene::SystemSceneID()))
                systemList.CreateSystem(mono_scene::SystemSceneID());

            systemList.SetOrder
            ({ 
                mono_d3d12::SystemWindowD3D12ID(),
                mono_scene::SystemSceneID()
            });
        }

        std::unique_ptr<riaecs::ISystemLoopCommand> Clone() const override
        {
            return std::make_unique<TitleSceneSystemListEditCmd>();
        }
    };

    class TitleSceneEntityFactory : public mono_scene::IEntitiesFactory
    {
    public:
        void CreateEntities(riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont) override
        {
            // Create title scene entities here
        }

        void DestroyEntities(riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont) override
        {
            // Destroy title scene entities here
        }
    };

    void CreateInitialEntities(riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont)
    {
        /***************************************************************************************************************
         * Create window
        /**************************************************************************************************************/

        riaecs::Entity windowEntity;
        {
            mono_d3d12::PrefabWindowD3D12 prefabWindow;
            prefabWindow.windowName_ = L"Test Window";
            prefabWindow.windowClassName_ = L"TestWindowClass";
            windowEntity = prefabWindow.Instantiate(ecsWorld, assetCont);
        }

        riaecs::Entity sceneEntity;
        {
            mono_scene::PrefabScene prefabScene;
            prefabScene.entitiesFactory_ = std::make_unique<TitleSceneEntityFactory>();
            prefabScene.assetSourceIDs_ = {};
            prefabScene.systemListEditCmds_.emplace_back(std::make_unique<TitleSceneSystemListEditCmd>());
            prefabScene.targetEditCmdIndex_ = 0;
            sceneEntity = prefabScene.Instantiate(ecsWorld, assetCont);
        }
    }

} // namespace mono_forge



int APIENTRY wWinMain
(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
){
    /*******************************************************************************************************************
     * Create standard console
    /******************************************************************************************************************/

#ifdef _DEBUG
    riaecs::CreateStandardConsole(L"Mono Forge Console");
#endif

    /*******************************************************************************************************************
     * Create ECS World
    /******************************************************************************************************************/
    
    std::unique_ptr<riaecs::IECSWorld> ecsWorld = std::make_unique<riaecs::ECSWorld>();
    ecsWorld->SetComponentFactoryRegistry(std::move(riaecs::gComponentFactoryRegistry));
    ecsWorld->SetComponentMaxCountRegistry(std::move(riaecs::gComponentMaxCountRegistry));
    ecsWorld->SetPoolFactory(std::make_unique<mem_alloc_fixed_block::FixedBlockPoolFactory>());
    ecsWorld->SetAllocatorFactory(std::make_unique<mem_alloc_fixed_block::FixedBlockAllocatorFactory>());
    ecsWorld->CreateWorld();

    /*******************************************************************************************************************
     * Create System loop
    /******************************************************************************************************************/

    std::unique_ptr<riaecs::ISystemLoop> systemLoop = std::make_unique<riaecs::SystemLoop>();
    systemLoop->SetSystemListFactory(std::make_unique<mono_forge::InitialSystemListFactory>());
    systemLoop->SetSystemLoopCommandQueueFactory(std::make_unique<riaecs::DefaultSystemLoopCommandQueueFactory>());
    systemLoop->Initialize();

    /*******************************************************************************************************************
     * Create Asset Container
    /******************************************************************************************************************/

    std::unique_ptr<riaecs::IAssetContainer> assetContainer = std::make_unique<riaecs::AssetContainer>();
    assetContainer->Create(riaecs::gAssetSourceRegistry->GetCount());

    /*******************************************************************************************************************
     * Create initialize entities
    /******************************************************************************************************************/

    mono_forge::CreateInitialEntities(*ecsWorld, *assetContainer);

    /*******************************************************************************************************************
     * Run game loop
    /******************************************************************************************************************/

    mono_forge::GameLoop gameLoop;
    gameLoop.Run(*systemLoop, *ecsWorld, *assetContainer);

    /*******************************************************************************************************************
     * Run message loop
    /******************************************************************************************************************/

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (!gameLoop.IsRunning())
            PostQuitMessage(0);
    }

    return 0;
}