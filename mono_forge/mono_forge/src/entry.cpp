#include "mono_forge/src/pch.h"

#pragma comment(lib, "riaecs.lib")
#pragma comment(lib, "mem_alloc_fixed_block.lib")

#include "mono_d3d12/mono_d3d12.h"
#pragma comment(lib, "mono_d3d12.lib")

#include "mono_forge/include/game_loop.h"

namespace mono_forge
{
    class InitialSystemListFactory : public riaecs::ISystemListFactory
    {
    public:
        std::unique_ptr<riaecs::ISystemList> Create() const override
        {
            std::unique_ptr<riaecs::ISystemList> systemList = std::make_unique<riaecs::SystemList>();
            systemList->Add(mono_d3d12::SystemWindowD3D12ID());

            return systemList;
        }

        void Destroy(std::unique_ptr<riaecs::ISystemList> product) const override
        {
            product.reset();
        }

        size_t GetProductSize() const override
        {
            return sizeof(riaecs::SystemList);
        }
    };

    void CreateInitialEntities(riaecs::IECSWorld &world, riaecs::IAssetContainer &assetCont)
    {
        /***************************************************************************************************************
         * Create window
        /**************************************************************************************************************/

        riaecs::Entity windowEntity;
        {
            mono_d3d12::PrefabWindowD3D12 prefabWindow;
            prefabWindow.SetWindowName(L"Test Window");
            prefabWindow.SetWindowClassName(L"TestWindowClass");
            windowEntity = prefabWindow.Instantiate(world, assetCont);
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
    systemLoop->SetSystemLoopCommandQueueFactory(std::make_unique<riaecs::EmptySystemLoopCommandQueueFactory>());
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