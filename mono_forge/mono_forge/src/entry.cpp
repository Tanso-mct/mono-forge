#include "mono_forge/src/pch.h"

#pragma comment(lib, "riaecs.lib")
#pragma comment(lib, "mem_alloc_fixed_block.lib")

int APIENTRY wWinMain
(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
){
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
     * Create Asset Container
    /******************************************************************************************************************/

    std::unique_ptr<riaecs::IAssetContainer> assetContainer = std::make_unique<riaecs::AssetContainer>();

    /*******************************************************************************************************************
     * Create initialize entities
    /******************************************************************************************************************/

    // TODO: Replace with actual entity creation and component addition logic

    /*******************************************************************************************************************
     * Create System loop and run it
    /******************************************************************************************************************/

    std::unique_ptr<riaecs::ISystemLoop> systemLoop = std::make_unique<riaecs::SystemLoop>();

    // TODO: Replace with actual system list factory
    systemLoop->SetSystemListFactory(std::make_unique<riaecs::EmptySystemListFactory>());

    systemLoop->SetSystemLoopCommandQueueFactory(std::make_unique<riaecs::EmptySystemLoopCommandQueueFactory>());

    // Execute the system loop
    systemLoop->Initialize();
    systemLoop->Run(*ecsWorld, *assetContainer);

    /*******************************************************************************************************************
     * Cleanup
    /******************************************************************************************************************/
    
    ecsWorld->DestroyWorld();

    return 0;
}