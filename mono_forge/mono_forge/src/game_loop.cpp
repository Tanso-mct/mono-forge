#include "mono_forge/src/pch.h"
#include "mono_forge/include/game_loop.h"

mono_forge::GameLoop::~GameLoop()
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    future_.get();
}

void mono_forge::GameLoop::Run(riaecs::ISystemLoop &systemLoop, riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont)
{
    future_ = std::async(std::launch::async, [&systemLoop, &ecsWorld, &assetCont, this]()
    {
        systemLoop.Run(ecsWorld, assetCont);

        {
            std::unique_lock<std::shared_mutex> lock(mutex_);
            isRunning_ = false;
        }
    });
}

bool mono_forge::GameLoop::IsRunning() const
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    return isRunning_;
}